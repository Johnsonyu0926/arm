// Filename: reloc_aarch64_efi.c

#include <efi.h>
#include <elf.h>

efi_status_t _relocate(long ldbase, Elf64_Dyn *dyn, efi_handle_t image,
                       struct efi_system_table *systab)
{
    long relsz = 0, relent = 0;
    Elf64_Rela *rel = 0;
    unsigned long *addr;
    int i;

    for (i = 0; dyn[i].d_tag != DT_NULL; ++i) {
        switch (dyn[i].d_tag) {
        case DT_RELA:
            rel = (Elf64_Rela *)((ulong)dyn[i].d_un.d_ptr + ldbase);
            break;
        case DT_RELASZ:
            relsz = dyn[i].d_un.d_val;
            break;
        case DT_RELAENT:
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
        switch (ELF64_R_TYPE(rel->r_info)) {
        case R_AARCH64_NONE:
            break;
        case R_AARCH64_RELATIVE:
            addr = (ulong *)(ldbase + rel->r_offset);
            *addr = ldbase + rel->r_addend;
            break;
        default:
            break;
        }
        rel = (Elf64_Rela *)((char *)rel + relent);
        relsz -= relent;
    }
    return EFI_SUCCESS;
}

// By GST @Date