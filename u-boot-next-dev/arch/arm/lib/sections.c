// Filename: sections.c

char __bss_start[0] __attribute__((section(".__bss_start")));
char __bss_end[0] __attribute__((section(".__bss_end")));
char __image_copy_start[0] __attribute__((section(".__image_copy_start")));
char __image_copy_end[0] __attribute__((section(".__image_copy_end")));
char __rel_dyn_start[0] __attribute__((section(".__rel_dyn_start")));
char __rel_dyn_end[0] __attribute__((section(".__rel_dyn_end")));
char __secure_start[0] __attribute__((section(".__secure_start")));
char __secure_end[0] __attribute__((section(".__secure_end")));
char __secure_stack_start[0] __attribute__((section(".__secure_stack_start")));
char __secure_stack_end[0] __attribute__((section(".__secure_stack_end")));
char __efi_runtime_start[0] __attribute__((section(".__efi_runtime_start")));
char __efi_runtime_stop[0] __attribute__((section(".__efi_runtime_stop")));
char __efi_runtime_rel_start[0] __attribute__((section(".__efi_runtime_rel_start")));
char __efi_runtime_rel_stop[0] __attribute__((section(".__efi_runtime_rel_stop")));
char _end[0] __attribute__((section(".__end")));

// By GST @Date