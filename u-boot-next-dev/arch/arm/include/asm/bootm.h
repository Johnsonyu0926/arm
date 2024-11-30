// Filename: bootm.h

#ifndef ARM_BOOTM_H
#define ARM_BOOTM_H

extern void udc_disconnect(void);

#if defined(CONFIG_SETUP_MEMORY_TAGS) || \
        defined(CONFIG_CMDLINE_TAG) || \
        defined(CONFIG_INITRD_TAG) || \
        defined(CONFIG_SERIAL_TAG) || \
        defined(CONFIG_REVISION_TAG)
# define BOOTM_ENABLE_TAGS       1
#else
# define BOOTM_ENABLE_TAGS       0
#endif

#ifdef CONFIG_SETUP_MEMORY_TAGS
# define BOOTM_ENABLE_MEMORY_TAGS    1
#else
# define BOOTM_ENABLE_MEMORY_TAGS    0
#endif

#ifdef CONFIG_CMDLINE_TAG
 #define BOOTM_ENABLE_CMDLINE_TAG    1
#else
 #define BOOTM_ENABLE_CMDLINE_TAG    0
#endif

#ifdef CONFIG_INITRD_TAG
 #define BOOTM_ENABLE_INITRD_TAG     1
#else
 #define BOOTM_ENABLE_INITRD_TAG     0
#endif

struct tag_serialnr;
#ifdef CONFIG_SERIAL_TAG
 #define BOOTM_ENABLE_SERIAL_TAG     1
void get_board_serial(struct tag_serialnr *serialnr);
#else
 #define BOOTM_ENABLE_SERIAL_TAG     0
static inline void get_board_serial(struct tag_serialnr *serialnr)
{
}
#endif

#ifdef CONFIG_REVISION_TAG
 #define BOOTM_ENABLE_REVISION_TAG   1
u32 get_board_rev(void);
#else
 #define BOOTM_ENABLE_REVISION_TAG   0
static inline u32 get_board_rev(void)
{
    return 0;
}
#endif

#endif // ARM_BOOTM_H

// By GST @Date