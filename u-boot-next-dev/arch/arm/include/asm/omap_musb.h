// Filename: omap_musb.h

#ifndef __ASM_ARM_OMAP_MUSB_H
#define __ASM_ARM_OMAP_MUSB_H

extern struct musb_platform_ops musb_dsps_ops;
extern const struct musb_platform_ops am35x_ops;
extern const struct musb_platform_ops omap2430_ops;

struct omap_musb_board_data {
    u8 interface_type;
    struct udevice *dev;
    void (*set_phy_power)(struct udevice *dev, u8 on);
    void (*clear_irq)(struct udevice *dev);
    void (*reset)(struct udevice *dev);
};

enum musb_interface {
    MUSB_INTERFACE_ULPI,
    MUSB_INTERFACE_UTMI
};

#endif /* __ASM_ARM_OMAP_MUSB_H */

// By GST @Date