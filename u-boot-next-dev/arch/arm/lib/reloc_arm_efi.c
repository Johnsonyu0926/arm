// Filename: reloc_arm_efi.c

#include <efi.h>
#include <elf.h>

efi_status_t _relocate(long ldbase, Elf32_Dyn *dyn, efi_handle_t image,
                       struct efi_system_table *systab)
{
    long relsz = 0, relent = 0;
    Elf32_Rel *rel = 0;
    ulong *addr;
    int i;

    for (i = 0; dyn[i].d_tag != DT_NULL; ++i) {
        switch (dyn[i].d_tag) {
        case DT_REL:
            rel = (Elf32_Rel *)((ulong)dyn[i].d_un.d_ptr + ldbase);
            break;
        case DT_RELSZ:
            relsz = dyn[i].d_un.d_val;
            break;
        case DT_RELENT:
            relent = dyn[i].d_un.d_val;
            break;
        default:
            break;
        }
    }

    if (!rel && relent == 0)
        return EFI_SUCCESS;

    if (!rel || relent == 0)
        return EFI_LOAD_ERROR;

    while (relsz > 0) {
        /* apply the relocs */
        switch (ELF32_R_TYPE(rel->r_info)) {
        case R_ARM_NONE:
            break;
        case R_ARM_RELATIVE:
            addr = (ulong *)(ldbase + rel->r_offset);
            *addr += ldbase;
            break;
        default:
            break;
        }
        rel = (Elf32_Rel *)((char *)rel + relent);
        relsz -= relent;
    }

    return EFI_SUCCESS;
}

// By GST @Date