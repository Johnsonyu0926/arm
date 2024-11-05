SECTION_DATA_PROLOGUE(.ramfunc,,)
{
    MPU_ALIGN(__ramfunc_size);
    __ramfunc_start = .;
    *(.ramfunc)
    *(".ramfunc.*")

#include <snippets-ramfunc-section.ld>

    MPU_ALIGN(__ramfunc_size);
    __ramfunc_end = .;
} GROUP_DATA_LINK_IN(RAMABLE_REGION, ROMABLE_REGION)
__ramfunc_size = __ramfunc_end - __ramfunc_start;
__ramfunc_load_start = LOADADDR(.ramfunc);
// By GST @Date