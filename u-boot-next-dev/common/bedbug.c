// Filename: bedbug.c

#include <common.h>
#include <linux/ctype.h>
#include <bedbug/bedbug.h>
#include <bedbug/ppc.h>
#include <bedbug/regs.h>
#include <bedbug/tables.h>

#define Elf32_Word    unsigned long

#ifdef USE_SOURCE_CODE
extern int line_info_from_addr __P ((Elf32_Word, char *, char *, int *));
extern struct symreflist *symByAddr;
extern char *symbol_name_from_addr __P ((Elf32_Word, int, int *));
#endif /* USE_SOURCE_CODE */

int print_operands __P ((struct ppc_ctx *));
int get_operand_value __P ((struct opcode *, unsigned long,
                            enum OP_FIELD, unsigned long *));
struct opcode *find_opcode __P ((unsigned long));
struct opcode *find_opcode_by_name __P ((char *));
char *spr_name __P ((int));
int spr_value __P ((char *));
char *tbr_name __P ((int));
int tbr_value __P ((char *));
int parse_operand __P ((unsigned long, struct opcode *,
                        struct operand *, char *, int *));
int get_word __P ((char **, char *));
long read_number __P ((char *));
int downstring __P ((char *));

int disppc (unsigned char *memaddr, unsigned char *virtual, int num_instr,
            int (*pfunc) (const char *), unsigned long flags)
{
    int i;
    struct ppc_ctx ctx;

#ifdef USE_SOURCE_CODE
    int line_no = 0;
    int last_line_no = 0;
    char funcname[128] = { 0 };
    char filename[256] = { 0 };
    char last_funcname[128] = { 0 };
    int symoffset;
    char *symname;
    char *cursym = (char *) 0;
#endif /* USE_SOURCE_CODE */
    /*------------------------------------------------------------*/

    ctx.flags = flags;
    ctx.virtual = virtual;

    /* Figure out the output radix before we go any further */

    if (ctx.flags & F_RADOCTAL) {
        /* Unsigned octal output */
        strcpy (ctx.radix_fmt, "O%o");
    } else if (ctx.flags & F_RADUDECIMAL) {
        /* Unsigned decimal output */
        strcpy (ctx.radix_fmt, "%u");
    } else if (ctx.flags & F_RADSDECIMAL) {
        /* Signed decimal output */
        strcpy (ctx.radix_fmt, "%d");
    } else {
        /* Unsigned hex output */
        strcpy (ctx.radix_fmt, "0x%x");
    }

    if (ctx.virtual == 0) {
        ctx.virtual = memaddr;
    }
#ifdef USE_SOURCE_CODE
    if (ctx.flags & F_SYMBOL) {
        if (symByAddr == 0)        /* no symbols loaded */
            ctx.flags &= ~F_SYMBOL;
        else {
            cursym = (char *) 0;
            symoffset = 0;
        }
    }
#endif /* USE_SOURCE_CODE */

    for (i = 0; i < num_instr; ++i, memaddr += 4, ctx.virtual += 4) {
#ifdef USE_SOURCE_CODE
        if (ctx.flags & F_LINENO) {
            if ((line_info_from_addr ((Elf32_Word) ctx.virtual,
                filename, funcname, &line_no) == true) &&
                ((line_no != last_line_no) ||
                 (strcmp (last_funcname, funcname) != 0))) {
                print_source_line (filename, funcname, line_no, pfunc);
            }
            last_line_no = line_no;
            strcpy (last_funcname, funcname);
        }
#endif /* USE_SOURCE_CODE */

        sprintf (ctx.data, "%08lx: ", (unsigned long) ctx.virtual);
        ctx.datalen = 10;

#ifdef USE_SOURCE_CODE
        if (ctx.flags & F_SYMBOL) {
            if ((symname =
                 symbol_name_from_addr((Elf32_Word) ctx.virtual,
                        true, 0)) != 0) {
                cursym = symname;
                symoffset = 0;
            } else {
                if ((cursym == 0) &&
                    ((symname =
                      symbol_name_from_addr((Elf32_Word) ctx.virtual,
                        false, &symoffset)) != 0)) {
                    cursym = symname;
                } else {
                    symoffset += 4;
                }
            }

            if (cursym != 0) {
                sprintf (&ctx.data[ctx.datalen], "<%s+", cursym);
                ctx.datalen = strlen (ctx.data);
                sprintf (&ctx.data[ctx.datalen], ctx.radix_fmt, symoffset);
                strcat (ctx.data, ">");
                ctx.datalen = strlen (ctx.data);
            }
        }
#endif /* USE_SOURCE_CODE */

        ctx.instr = INSTRUCTION (memaddr);

        if (ctx.flags & F_INSTR) {
            /* Find the opcode structure for this opcode.  If one is not found
               then it must be an illegal instruction */
            sprintf (&ctx.data[ctx.datalen],
                     "   %02lx %02lx %02lx %02lx    ",
                     ((ctx.instr >> 24) & 0xff),
                     ((ctx.instr >> 16) & 0xff), ((ctx.instr >> 8) & 0xff),
                     (ctx.instr & 0xff));
            ctx.datalen += 18;
        } else {
            strcat (ctx.data, "   ");
            ctx.datalen += 3;
        }

        if ((ctx.op = find_opcode (ctx.instr)) == 0) {
            /* Illegal Opcode */
            sprintf (&ctx.data[ctx.datalen], "        .long 0x%08lx",
                     ctx.instr);
            ctx.datalen += 24;
            (*pfunc) (ctx.data);
            continue;
        }

        if (((ctx.flags & F_SIMPLE) == 0) ||
            (ctx.op->hfunc == 0) ||
            ((*ctx.op->hfunc) (&ctx) == false)) {
            sprintf (&ctx.data[ctx.datalen], "%-7s ", ctx.op->name);
            ctx.datalen += 8;
            print_operands (&ctx);
        }

        (*pfunc) (ctx.data);
    }

    return true;
}                                /* disppc */

int print_operands (struct ppc_ctx *ctx)
{
    int open_parens = 0;
    int field;
    unsigned long operand;
    struct operand *opr;

#ifdef USE_SOURCE_CODE
    char *symname;
    int offset;
#endif /* USE_SOURCE_CODE */
    /*------------------------------------------------------------*/

    /* Walk through the operands and list each in order */
    for (field = 0; ctx->op->fields[field] != 0; ++field) {
        if (ctx->op->fields[field] > n_operands) {
            continue;            /* bad operand ?! */
        }

        opr = &operands[ctx->op->fields[field] - 1];

        if (opr->hint & OH_SILENT) {
            continue;
        }

        if ((field > 0) && !open_parens) {
            strcat (ctx->data, ",");
            ctx->datalen++;
        }

        operand = (ctx->instr >> opr->shift) & ((1 << opr->bits) - 1);

        if (opr->hint & OH_ADDR) {
            if ((operand & (1 << (opr->bits - 1))) != 0) {
                operand = operand - (1 << opr->bits);
            }

            if (ctx->op->hint & H_RELATIVE)
                operand = (operand << 2) + (unsigned long) ctx->virtual;
            else
                operand = (operand << 2);

            sprintf (&ctx->data[ctx->datalen], "0x%lx", operand);
            ctx->datalen = strlen (ctx->data);

#ifdef USE_SOURCE_CODE
            if ((ctx->flags & F_SYMBOL) &&
                ((symname =
                  symbol_name_from_addr (operand, 0, &offset)) != 0)) {
                sprintf (&ctx->data[ctx->datalen], " <%s", symname);
                if (offset != 0) {
                    strcat (ctx->data, "+");
                    ctx->datalen = strlen (ctx->data);
                    sprintf (&ctx->data[ctx->datalen], ctx->radix_fmt,
                             offset);
                }
                strcat (ctx->data, ">");
            }
#endif /* USE_SOURCE_CODE */
        }

        else if (opr->hint & OH_REG) {
            if ((operand == 0) &&
                (opr->field == O_rA) && (ctx->op->hint & H_RA0_IS_0)) {
                strcat (ctx->data, "0");
            } else {
                sprintf (&ctx->data[ctx->datalen], "r%d", (short) operand);
            }

            if (open_parens) {
                strcat (ctx->data, ")");
                open_parens--;
            }
        }

        else if (opr->hint & OH_SPR) {
            strcat (ctx->data, spr_name (operand));
        }

        else if (opr->hint & OH_TBR) {
            strcat (ctx->data, tbr_name (operand));
        }

        else if (opr->hint & OH_LITERAL) {
            switch (opr->field) {
            case O_cr2:
                strcat (ctx->data, "cr2");
                ctx->datalen += 3;
                break;

            default:
                break;
            }
        }

        else {
            sprintf (&ctx->data[ctx->datalen], ctx->radix_fmt,
                     (unsigned short) operand);

            if (open_parens) {
                strcat (ctx->data, ")");
                open_parens--;
            }

            else if (opr->hint & OH_OFFSET) {
                strcat (ctx->data, "(");
                open_parens++;
            }
        }

        ctx->datalen = strlen (ctx->data);
    }

    return 0;
}                                /* print_operands */

int get_operand_value (struct opcode *op, unsigned long instr,
                       enum OP_FIELD field, unsigned long *value)
{
    int i;
    struct operand *opr;

    /*------------------------------------------------------------*/

    if (field > n_operands) {
        return false;            /* bad operand ?! */
    }

    /* Walk through the operands and list each in order */
    for (i = 0; op->fields[i] != 0; ++i) {
        if (op->fields[i] != field) {
            continue;
        }

        opr = &operands[op->fields[i] - 1];

        if (value) {
            *value = (instr >> opr->shift) & ((1 << opr->bits) - 1);
        }
        return true;
    }

    return false;
}                                /* operand_value */

struct opcode *find_opcode (unsigned long instr)
{
    struct opcode *ptr;
    int top = 0;
    int bottom = n_opcodes - 1;
    int idx;

    /*------------------------------------------------------------*/

    while (top <= bottom) {
        idx = (top + bottom) >> 1;
        ptr = &opcodes[idx];

        if ((instr & ptr->mask) < ptr->opcode) {
            bottom = idx - 1;
        } else if ((instr & ptr->mask) > ptr->opcode) {
            top = idx + 1;
        } else {
            return ptr;
        }
    }

    return (struct opcode *) 0;
}                                /* find_opcode */

struct opcode *find_opcode_by_name (char *name)
{
    int idx;

    /*------------------------------------------------------------*/

    downstring (name);

    for (idx = 0; idx < n_opcodes; ++idx) {
        if (!strcmp (name, opcodes[idx].name))
            return &opcodes[idx];
    }

    return (struct opcode *) 0;
}                                /* find_opcode_by_name */

char *spr_name (int value)
{
    unsigned short spr;
    static char other[10];
    int i;

    spr = ((value >> 5) & 0x1f) | ((value & 0x1f) << 5);

    for (i = 0; i < n_sprs; ++i) {
        if (spr == spr_map[i].spr_val)
            return spr_map[i].spr_name;
    }

    sprintf (other, "%d", spr);
    return other;
}                                /* spr_name */

int spr_value (char *name)
{
    struct spr_info *sprp;
    int spr;
    int i;

    /*------------------------------------------------------------*/

    if (!name || !*name)
        return 0;

    if (isdigit ((int) name[0])) {
        i = htonl (read_number (name));
        spr = ((i >> 5) & 0x1f) | ((i & 0x1f) << 5);
        return spr;
    }

    downstring (name);

    for (i = 0; i < n_sprs; ++i) {
        sprp = &spr_map[i];

        if (strcmp (name, sprp->spr_name) == 0) {
            /* spr is a 10 bit field whose interpretation has the high and low
               five-bit fields reversed from their encoding in the operand */
            i = htonl (sprp->spr_val);
            spr = ((i >> 5) & 0x1f) | ((i & 0x1f) << 5);

            return spr;
        }
    }

    return 0;
}                                /* spr_value */

char *tbr_name (int value)
{
    unsigned short tbr;

    tbr = ((value >> 5) & 0x1f) | ((value & 0x1f) << 5);

    if (tbr == 268)
        return "TBL";

    else if (tbr == 269)
        return "TBU";

    return "???";
}                                /* tbr_name */

int tbr_value (char *name)
{
    int tbr;
    int val;

    if (!name || !*name)
        return 0;

    downstring (name);

    if (isdigit ((int) name[0])) {
        val = read_number (name);

        if (val != 268 && val != 269)
            return 0;
    } else if (strcmp (name, "tbl") == 0)
        val = 268;
    else if (strcmp (name, "tbu") == 0)
        val = 269;
    else
        return 0;

    val = htonl (val);
    tbr = ((val >> 5) & 0x1f) | ((val & 0x1f) << 5);
    return tbr;
}                                /* tbr_name */

int handle_bc (struct ppc_ctx *ctx)
{
    unsigned long bo;
    unsigned long bi;
    static struct opcode blt = { B_OPCODE (16, 0, 0), B_MASK, {O_BD, 0},
    0, "blt", H_RELATIVE
    };
    static struct opcode bne =
            { B_OPCODE (16, 0, 0), B_MASK, {O_cr2, O_BD, 0},
    0, "bne", H_RELATIVE
    };
    static struct opcode bdnz = { B_OPCODE (16, 0, 0), B_MASK, {O_BD, 0},
    0, "bdnz", H_RELATIVE
    };

    if (get_operand_value(ctx->op, ctx->instr, O_BO, &bo) == false)
        return false;

    if (get_operand_value(ctx->op, ctx->instr, O_BI, &bi) == false)
        return false;

    if ((bo == 12) && (bi == 0)) {
        ctx->op = &blt;
        sprintf (&ctx->data[ctx->datalen], "%-7s ", ctx->op->name);
        ctx->datalen += 8;
        print_operands (ctx);
        return true;
    } else if ((bo == 4) && (bi == 10)) {
        ctx->op = &bne;
        sprintf (&ctx->data[ctx->datalen], "%-7s ", ctx->op->name);
        ctx->datalen += 8;
        print_operands (ctx);
        return true;
    } else if ((bo == 16) && (bi == 0)) {
        ctx->op = &bdnz;
        sprintf (&ctx->data[ctx->datalen], "%-7s ", ctx->op->name);
        ctx->datalen += 8;
        print_operands (ctx);
        return true;
    }

    return false;
}                                /* handle_blt */

int print_source_line (char *filename, char *funcname,
                       int line_no, int (*pfunc) (const char *))
{
    char out_buf[256];

    (*pfunc) ("");                /* output a newline */
    sprintf (out_buf, "%s %s(): line %d", filename, funcname, line_no);
    (*pfunc) (out_buf);

    return true;
}                                /* print_source_line */

unsigned long asmppc (unsigned long memaddr, char *asm_buf, int *err)
{
    struct opcode *opc;
    struct operand *oper[MAX_OPERANDS];
    unsigned long instr;
    unsigned long param;
    char *ptr = asm_buf;
    char scratch[20];
    int i;
    int w_operands = 0;            /* wanted # of operands */
    int n_operands = 0;            /* # of operands read */
    int asm_debug = 0;

    if (err)
        *err = 0;

    if (get_word (&ptr, scratch) == 0)
        return 0;

    if ((opc = find_opcode_by_name (scratch)) == (struct opcode *) 0) {
        if (err)
            *err = E_ASM_BAD_OPCODE;
        return 0;
    }

    if (asm_debug) {
        printf ("asmppc: Opcode = \"%s\"\n", opc->name);
    }

    for (i = 0; i < 8; ++i) {
        if (opc->fields[i] == 0)
            break;
        ++w_operands;
    }

    if (asm_debug) {
        printf ("asmppc: Expecting %d operands\n", w_operands);
    }

    instr = opc->opcode;

    while (n_operands < w_operands) {
        oper[n_operands] = &operands[opc->fields[n_operands] - 1];

        if (oper[n_operands]->hint & OH_SILENT) {
            if (asm_debug) {
                printf ("asmppc: Operand %d \"%s\" SILENT\n", n_operands,
                        oper[n_operands]->name);
            }
            ++n_operands;
            continue;
        }

        if (get_word (&ptr, scratch) == 0)
            break;

        if (asm_debug) {
            printf ("asmppc: Operand %d \"%s\" : \"%s\"\n", n_operands,
                    oper[n_operands]->name, scratch);
        }

        if ((param = parse_operand (memaddr, opc, oper[n_operands],
                                    scratch, err)) == -1)
            return 0;

        instr |= param;
        ++n_operands;
    }

    if (n_operands < w_operands) {
        if (err)
            *err = E_ASM_NUM_OPERANDS;
        return 0;
    }

    if (asm_debug) {
        printf ("asmppc: Instruction = 0x%08lx\n", instr);
    }

    return instr;
}                                /* asmppc */

int parse_operand (unsigned long memaddr, struct opcode *opc,
                   struct operand *oper, char *txt, int *err)
{
    long data;
    long mask;
    int is_neg = 0;

    mask = (1 << oper->bits) - 1;

    if (oper->hint & OH_ADDR) {
        data = read_number (txt);

        if (opc->hint & H_RELATIVE)
            data = data - memaddr;

        if (data < 0)
            is_neg = 1;

        data >>= 2;
        data &= (mask >> 1);

        if (is_neg)
            data |= 1 << (oper->bits - 1);
    }

    else if (oper->hint & OH_REG) {
        if (txt[0] == 'r' || txt[0] == 'R')
            txt++;
        else if (txt[0] == '%' && (txt[1] == 'r' || txt[1] == 'R'))
            txt += 2;

        data = read_number (txt);
        if (data > 31) {
            if (err)
                *err = E_ASM_BAD_REGISTER;
            return -1;
        }

        data = htonl (data);
    }

    else if (oper->hint & OH_SPR) {
        if ((data = spr_value (txt)) == 0) {
            if (err)
                *err = E_ASM_BAD_SPR;
            return -1;
        }
    }

    else if (oper->hint & OH_TBR) {
        if ((data = tbr_value (txt)) == 0) {
            if (err)
                *err = E_ASM_BAD_TBR;
            return -1;
        }
    }

    else {
        data = htonl (read_number (txt));
    }

    return (data & mask) << oper->shift;
}                                /* parse_operand */

char *asm_error_str (int err)
{
    switch (err) {
    case E_ASM_BAD_OPCODE:
        return "Bad opcode";
    case E_ASM_NUM_OPERANDS:
        return "Bad number of operands";
    case E_ASM_BAD_REGISTER:
        return "Bad register number";
    case E_ASM_BAD_SPR:
        return "Bad SPR name or number";
    case E_ASM_BAD_TBR:
        return "Bad TBR name or number";
    }

    return "";
}                                /* asm_error_str */

int get_word (char **src, char *dest)
{
    char *ptr = *src;
    int nchars = 0;

    while (*ptr && isblank (*ptr))
        ptr++;

    if (*ptr == 0) {
        *src = ptr;
        return 0;
    }

    while (*ptr && !isblank (*ptr) && (*ptr != ','))
        dest[nchars++] = *ptr++;
    ptr = (*ptr == ',') ? ptr + 1 : ptr;
    dest[nchars] = 0;

    *src = ptr;
    return nchars;
}                                /* get_word */

long read_number (char *txt)
{
    long val;
    int is_neg = 0;

    if (txt == 0 || *txt == 0)
        return 0;

    if (*txt == '-') {
        is_neg = 1;
        ++txt;
    }

    if (txt[0] == '0' && (txt[1] == 'x' || txt[1] == 'X'))    /* hex */
        val = simple_strtoul (&txt[2], NULL, 16);
    else                        /* decimal */
        val = simple_strtoul (txt, NULL, 10);

    if (is_neg)
        val = -val;

    return val;
}                                /* read_number */

int downstring (char *s)
{
    if (!s || !*s)
        return 0;

    while (*s) {
        if (isupper (*s))
            *s = tolower (*s);
        s++;
    }

    return 0;
}                                /* downstring */

int find_next_address (unsigned char *nextaddr, int step_over,
                       struct pt_regs *regs)
{
    unsigned long pc;            /* SRR0 register from PPC */
    unsigned long ctr;            /* CTR register from PPC */
    unsigned long cr;            /* CR register from PPC */
    unsigned long lr;            /* LR register from PPC */
    unsigned long instr;        /* instruction at SRR0 */
    unsigned long next;            /* computed instruction for 'next' */
    unsigned long step;            /* computed instruction for 'step' */
    unsigned long addr = 0;        /* target address operand */
    unsigned long aa = 0;        /* AA operand */
    unsigned long lk = 0;        /* LK operand */
    unsigned long bo = 0;        /* BO operand */
    unsigned long bi = 0;        /* BI operand */
    struct opcode *op = 0;        /* opcode structure for 'instr' */
    int ctr_ok = 0;
    int cond_ok = 0;
    int conditional = 0;
    int branch = 0;

    if (nextaddr == 0 || regs == 0) {
        printf ("find_next_address: bad args");
        return false;
    }

    pc = regs->nip & 0xfffffffc;
    instr = INSTRUCTION (pc);

    if ((op = find_opcode (instr)) == (struct opcode *) 0) {
        printf ("find_next_address: can't parse opcode 0x%lx", instr);
        return false;
    }

    ctr = regs->ctr;
    cr = regs->ccr;
    lr = regs->link;

    switch (op->opcode) {
    case B_OPCODE (16, 0, 0):    /* bc */
    case B_OPCODE (16, 0, 1):    /* bcl */
    case B_OPCODE (16, 1, 0):    /* bca */
    case B_OPCODE (16, 1, 1):    /* bcla */
        if (!get_operand_value (op, instr, O_BD, &addr) ||
            !get_operand_value (op, instr, O_BO, &bo) ||
            !get_operand_value (op, instr, O_BI, &bi) ||
            !get_operand_value (op, instr, O_AA, &aa) ||
            !get_operand_value (op, instr, O_LK, &lk))
            return false;

        if ((addr & (1 << 13)) != 0)
            addr = addr - (1 << 14);
        addr <<= 2;
        conditional = 1;
        branch = 1;
        break;

    case I_OPCODE (18, 0, 0):    /* b */
    case I_OPCODE (18, 0, 1):    /* bl */
    case I_OPCODE (18, 1, 0):    /* ba */
    case I_OPCODE (18, 1, 1):    /* bla */
        if (!get_operand_value (op, instr, O_LI, &addr) ||
            !get_operand_value (op, instr, O_AA, &aa) ||
            !get_operand_value (op, instr, O_LK, &lk))
            return false;

        if ((addr & (1 << 23)) != 0)
            addr = addr - (1 << 24);
        addr <<= 2;
        conditional = 0;
        branch = 1;
        break;

    case XL_OPCODE (19, 528, 0):    /* bcctr */
    case XL_OPCODE (19, 528, 1):    /* bcctrl */
        if (!get_operand_value (op, instr, O_BO, &bo) ||
            !get_operand_value (op, instr, O_BI, &bi) ||
            !get_operand_value (op, instr, O_LK, &lk))
            return false;

        addr = ctr;
        aa = 1;
        conditional = 1;
        branch = 1;
        break;

    case XL_OPCODE (19, 16, 0):    /* bclr */
    case XL_OPCODE (19, 16, 1):    /* bclrl */
        if (!get_operand_value (op, instr, O_BO, &bo) ||
            !get_operand_value (op, instr, O_BI, &bi) ||
            !get_operand_value (op, instr, O_LK, &lk))
            return false;

        addr = lr;
        aa = 1;
        conditional = 1;
        branch = 1;
        break;

    default:
        conditional = 0;
        branch = 0;
        break;
    }

    if (conditional) {
        switch ((bo & 0x1e) >> 1) {
        case 0:                /* 0000y */
            if (--ctr != 0)
                ctr_ok = 1;

            cond_ok = !(cr & (1 << (31 - bi)));
            break;

        case 1:                /* 0001y */
            if (--ctr == 0)
                ctr_ok = 1;

            cond_ok = !(cr & (1 << (31 - bi)));
            break;

        case 2:                /* 001zy */
            ctr_ok = 1;
            cond_ok = !(cr & (1 << (31 - bi)));
            break;

        case 4:                /* 0100y */
            if (--ctr != 0)
                ctr_ok = 1;

            cond_ok = cr & (1 << (31 - bi));
            break;

        case 5:                /* 0101y */
            if (--ctr == 0)
                ctr_ok = 1;

            cond_ok = cr & (1 << (31 - bi));
            break;

        case 6:                /* 011zy */
            ctr_ok = 1;
            cond_ok = cr & (1 << (31 - bi));
            break;

        case 8:                /* 1z00y */
            if (--ctr != 0)
                ctr_ok = cond_ok = 1;
            break;

        case 9:                /* 1z01y */
            if (--ctr == 0)
                ctr_ok = cond_ok = 1;
            break;

        case 10:                /* 1z1zz */
            ctr_ok = cond_ok = 1;
            break;
        }
    }

    if (branch && (!conditional || (ctr_ok && cond_ok))) {
        if (aa)
            step = addr;
        else
            step = addr + pc;

        if (lk)
            next = pc + 4;
        else
            next = step;
    } else {
        step = next = pc + 4;
    }

    if (step_over == true)
        *(unsigned long *) nextaddr = next;
    else
        *(unsigned long *) nextaddr = step;

    return true;
}

// By GST @Date