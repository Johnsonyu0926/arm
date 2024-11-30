// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * rohm-bu18rl82.c  --  I2C register interface access for bu18rl82 serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "../core.h"
#include "rohm-bu18rl82.h"

static int BU18RL82_GPIO0_pins[] = {0};
static int BU18RL82_GPIO1_pins[] = {1};
static int BU18RL82_GPIO2_pins[] = {2};
static int BU18RL82_GPIO3_pins[] = {3};
static int BU18RL82_GPIO4_pins[] = {4};
static int BU18RL82_GPIO5_pins[] = {5};
static int BU18RL82_GPIO6_pins[] = {6};
static int BU18RL82_GPIO7_pins[] = {7};

#define GROUP_DESC(nm) \
{ \
	.name = #nm, \
	.pins = nm ## _pins, \
	.num_pins = ARRAY_SIZE(nm ## _pins), \
}

struct serdes_function_data {
	u8 gpio_rx_en:1;
	u16 gpio_id;
	u16 mdelay;
};

static const char *serdes_gpio_groups[] = {
	"BU18RL82_GPIO0", "BU18RL82_GPIO1", "BU18RL82_GPIO2", "BU18RL82_GPIO3",
	"BU18RL82_GPIO4", "BU18RL82_GPIO5", "BU18RL82_GPIO6", "BU18RL82_GPIO7",
};

/*des -> ser -> soc*/
#define FUNCTION_DESC_GPIO_INPUT(id) \
{ \
	.name = "DES_TO_SER_GPIO"#id, \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en = 1, .gpio_id = id + 2 } \
	}, \
} \

/*soc -> ser -> des*/
#define FUNCTION_DESC_GPIO_OUTPUT(id) \
{ \
	.name = "SER_GPIO"#id"_TO_DES", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en = 0, .gpio_id = id + 2 } \
	}, \
} \

#define FUNCTION_DES_DELAY_MS(ms) \
{ \
	.name = "DELAY_"#ms"MS", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .mdelay = ms, } \
	}, \
} \

/*des -> device*/
#define FUNCTION_DESC_GPIO_OUTPUT_HIGH() \
{ \
	.name = "DES_GPIO_OUTPUT_HIGH", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en = 0, .gpio_id = 1 } \
	}, \
} \

#define FUNCTION_DESC_GPIO_OUTPUT_LOW() \
{ \
	.name = "DES_GPIO_OUTPUT_LOW", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en = 0, .gpio_id = 0 } \
	}, \
} \

static struct pinctrl_pin_desc bu18rl82_pins_desc[] = {
	PINCTRL_PIN(ROHM_BU18RL82_GPIO0, "BU18RL82_GPIO0"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO1, "BU18RL82_GPIO1"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO2, "BU18RL82_GPIO2"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO3, "BU18RL82_GPIO3"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO4, "BU18RL82_GPIO4"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO5, "BU18RL82_GPIO5"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO6, "BU18RL82_GPIO6"),
	PINCTRL_PIN(ROHM_BU18RL82_GPIO7, "BU18RL82_GPIO7"),
};

static struct group_desc bu18rl82_groups_desc[] = {
	GROUP_DESC(BU18RL82_GPIO0),
	GROUP_DESC(BU18RL82_GPIO1),
	GROUP_DESC(BU18RL82_GPIO2),
	GROUP_DESC(BU18RL82_GPIO3),
	GROUP_DESC(BU18RL82_GPIO4),
	GROUP_DESC(BU18RL82_GPIO5),
	GROUP_DESC(BU18RL82_GPIO6),
	GROUP_DESC(BU18RL82_GPIO7),
};

static struct function_desc bu18rl82_functions_desc[] = {
	FUNCTION_DESC_GPIO_INPUT(0),
	FUNCTION_DESC_GPIO_INPUT(1),
	FUNCTION_DESC_GPIO_INPUT(2),
	FUNCTION_DESC_GPIO_INPUT(3),
	FUNCTION_DESC_GPIO_INPUT(4),
	FUNCTION_DESC_GPIO_INPUT(5),
	FUNCTION_DESC_GPIO_INPUT(6),
	FUNCTION_DESC_GPIO_INPUT(7),
	FUNCTION_DESC_GPIO_INPUT(8),
	FUNCTION_DESC_GPIO_INPUT(9),
	FUNCTION_DESC_GPIO_INPUT(10),
	FUNCTION_DESC_GPIO_INPUT(11),
	FUNCTION_DESC_GPIO_INPUT(12),
	FUNCTION_DESC_GPIO_INPUT(13),
	FUNCTION_DESC_GPIO_INPUT(14),
	FUNCTION_DESC_GPIO_INPUT(15),

	FUNCTION_DESC_GPIO_OUTPUT(0),
	FUNCTION_DESC_GPIO_OUTPUT(1),
	FUNCTION_DESC_GPIO_OUTPUT(2),
	FUNCTION_DESC_GPIO_OUTPUT(3),
	FUNCTION_DESC_GPIO_OUTPUT(4),
	FUNCTION_DESC_GPIO_OUTPUT(5),
	FUNCTION_DESC_GPIO_OUTPUT(6),
	FUNCTION_DESC_GPIO_OUTPUT(7),
	FUNCTION_DESC_GPIO_OUTPUT(8),
	FUNCTION_DESC_GPIO_OUTPUT(9),
	FUNCTION_DESC_GPIO_OUTPUT(10),
	FUNCTION_DESC_GPIO_OUTPUT(11),
	FUNCTION_DESC_GPIO_OUTPUT(12),
	FUNCTION_DESC_GPIO_OUTPUT(13),
	FUNCTION_DESC_GPIO_OUTPUT(14),
	FUNCTION_DESC_GPIO_OUTPUT(15),

	FUNCTION_DESC_GPIO_OUTPUT_HIGH(),
	FUNCTION_DESC_GPIO_OUTPUT_LOW(),

	FUNCTION_DES_DELAY_MS(10),
	FUNCTION_DES_DELAY_MS(50),
	FUNCTION_DES_DELAY_MS(100),
	FUNCTION_DES_DELAY_MS(200),
	FUNCTION_DES_DELAY_MS(500),
};

static struct serdes_chip_pinctrl_info bu18rl82_pinctrl_info = {
	.pins = bu18rl82_pins_desc,
	.num_pins = ARRAY_SIZE(bu18rl82_pins_desc),
	.groups = bu18rl82_groups_desc,
	.num_groups = ARRAY_SIZE(bu18rl82_groups_desc),
	.functions = bu18rl82_functions_desc,
	.num_functions = ARRAY_SIZE(bu18rl82_functions_desc),
};

void bu18rl82_bridge_swrst(struct serdes *serdes)
{
	int ret;

	ret = serdes_reg_write(serdes, BU18RL82_REG_RESET,
			       BIT(0) | BIT(1) | BIT(7));
	if (ret < 0)
		printf("%s: failed to reset bu18rl82 0x11 ret=%d\n",
		       __func__, ret);

	mdelay(20);

	SERDES_DBG_CHIP("%s: serdes %s ret=%d\n",
			__func__, serdes->dev->name, ret);
}

void bu18rl82_enable_hwint(struct serdes *serdes, int enable)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(bu18rl82_reg_ien); i++) {
		if (enable) {
			ret = serdes_reg_write(serdes, bu18rl82_reg_ien[i].reg,
					       bu18rl82_reg_ien[i].ien);
			if (ret)
				printf("reg 0x%04x write error\n",
				       bu18rl82_reg_ien[i].reg);
		} else {
			ret = serdes_reg_write(serdes, bu18rl82_reg_ien[i].reg, 0);
			if (ret)
				printf("reg 0x%04x write error\n",
				       bu18rl82_reg_ien[i].reg);
		}
	}

	SERDES_DBG_CHIP("%s: serdes %s ret=%d\n",
			__func__,
			serdes->dev->name, enable);
}

int bu18rl82_bridge_init(struct serdes *serdes)
{
	if (!dm_gpio_is_valid(&serdes->reset_gpio)) {
		bu18rl82_bridge_swrst(serdes);
		SERDES_DBG_CHIP("%s: serdes %s\n", __func__,
				serdes->dev->name);
	}

	return 0;
}

int bu18rl82_bridge_enable(struct serdes *serdes)
{
	int ret = 0;

	/* 1:enable 0:dsiable*/
	bu18rl82_enable_hwint(serdes, 0);

	SERDES_DBG_CHIP("%s: serdes %s ret=%d\n", __func__,
			serdes->dev->name, ret);
	return ret;
}

int bu18rl82_bridge_disable(struct serdes *serdes)
{
	return 0;
}

static struct serdes_chip_bridge_ops bu18rl82_bridge_ops = {
	.init = bu18rl82_bridge_init,
	.enable = bu18rl82_bridge_enable,
	.disable = bu18rl82_bridge_disable,
};

static int bu18rl82_pinctrl_config_set(struct serdes *serdes,
				       unsigned int pin_selector,
				       unsigned int param, unsigned int argument)
{
	switch (param) {
	case PIN_CONFIG_DRIVE_STRENGTH:
		serdes_set_bits(serdes, bu18rl82_gpio_sw[pin_selector].reg,
				bu18rl82_gpio_sw[pin_selector].mask,
				FIELD_PREP(BIT(2) | BIT(1), argument));
		SERDES_DBG_CHIP("%s: serdes %s pin=%d drive arg=0x%x\n",
				__func__,
				serdes->dev->name, pin_selector, argument);
		break;
	case PIN_CONFIG_BIAS_PULL_DOWN:
		serdes_set_bits(serdes, bu18rl82_gpio_pden[pin_selector].reg,
				bu18rl82_gpio_pden[pin_selector].mask,
				FIELD_PREP(BIT(4), argument));
		SERDES_DBG_CHIP("%s: serdes %s pin=%d pull-down arg=0x%x\n",
				__func__,
				serdes->dev->name, pin_selector, argument);
		break;
	case PIN_CONFIG_OUTPUT:
		serdes_set_bits(serdes, bu18rl82_gpio_oen[pin_selector].reg,
				bu18rl82_gpio_oen[pin_selector].mask,
				FIELD_PREP(BIT(3), argument));
		SERDES_DBG_CHIP("%s: serdes %s pin=%d output arg=0x%x\n",
				__func__,
				serdes->dev->name, pin_selector, argument);
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int bu18rl82_pinctrl_set_pin_mux(struct serdes *serdes,
					unsigned int pin_selector,
					unsigned int func_selector)
{
	struct function_desc *func;
	struct pinctrl_pin_desc *pin;
	int offset;
	u16 ms;

	func = &serdes->chip_data->pinctrl_info->functions[func_selector];
	if (!func) {
		printf("%s: func is null\n", __func__);
		return -EINVAL;
	}

	pin = &serdes->chip_data->pinctrl_info->pins[pin_selector];
	if (!pin) {
		printf("%s: pin is null\n", __func__);
		return -EINVAL;
	}

	SERDES_DBG_CHIP("%s: serdes %s func=%s data=%p pin=%s num=%d\n",
			__func__, serdes->dev->name,
			func->name, func->data,
			pin->name, pin->number);

	if (func->data) {
		struct serdes_function_data *fdata = func->data;
		ms = fdata->mdelay;

		offset = pin->number;
		if (offset > 7)
			dev_err(serdes->dev, "%s offset=%d > 7\n",
				serdes->dev->name, offset);
		else
			SERDES_DBG_CHIP("%s: serdes %s id=0x%x off=%d\n",
					__func__, serdes->dev->name,
					fdata->gpio_id, offset);
		if (!ms) {
			serdes_set_bits(serdes, bu18rl82_gpio_oen[offset].reg,
					bu18rl82_gpio_oen[offset].mask,
					FIELD_PREP(BIT(3), fdata->gpio_rx_en));
			serdes_set_bits(serdes, bu18rl82_gpio_id_low[offset].reg,
					bu18rl82_gpio_id_low[offset].mask,
					FIELD_PREP(GENMASK(7, 0),
					(fdata->gpio_id & 0xff)));
			serdes_set_bits(serdes, bu18rl82_gpio_id_high[offset].reg,
					bu18rl82_gpio_id_high[offset].mask,
					FIELD_PREP(GENMASK(2, 0),
					((fdata->gpio_id >> 8) & 0x7)));
			serdes_set_bits(serdes, bu18rl82_gpio_pden[offset].reg,
					bu18rl82_gpio_pden[offset].mask,
					FIELD_PREP(BIT(4), 0));
		} else {
			mdelay(ms);
			SERDES_DBG_CHIP("%s: delay %dms\n", __func__, ms);
		}
	}

	return 0;
}

static int bu18rl82_pinctrl_set_grp_mux(struct serdes *serdes,
					unsigned int group_selector,
					unsigned int func_selector)
{
	struct serdes_pinctrl *pinctrl = serdes->serdes_pinctrl;
	struct function_desc *func;
	struct group_desc *grp;
	int i, offset;

	func = &serdes->chip_data->pinctrl_info->functions[func_selector];
	if (!func) {
		printf("%s: func is null\n", __func__);
		return -EINVAL;
	}

	grp = &serdes->chip_data->pinctrl_info->groups[group_selector];
	if (!grp) {
		printf("%s: grp is null\n", __func__);
		return -EINVAL;
	}

	SERDES_DBG_CHIP("%s: serdes %s func=%s data=%p grp=%s data=%p, num=%d\n",
			__func__, serdes->chip_data->name, func->name,
			func->data, grp->name, grp->data, grp->num_pins);

	if (func->data) {
		struct serdes_function_data *fdata = func->data;

		for (i = 0; i < grp->num_pins; i++) {
			offset = grp->pins[i] - pinctrl->pin_base;
			if (offset > 7)
				dev_err(serdes->dev, "%s offset=%d > 7\n",
					serdes->dev->name, offset);
			else
				SERDES_DBG_CHIP("%s: serdes %s id=0x%x, offset=%d\n",
						__func__,
						serdes->dev->name, fdata->gpio_id, offset);
			serdes_set_bits(serdes, bu18rl82_gpio_oen[offset].reg,
					bu18rl82_gpio_oen[offset].mask,
					FIELD_PREP(BIT(3), fdata->gpio_rx_en));
			serdes_set_bits(serdes, bu18rl82_gpio_id_low[offset].reg,
					bu18rl82_gpio_id_low[offset].mask,
					FIELD_PREP(GENMASK(7, 0),
					(fdata->gpio_id & 0xff)));
			serdes_set_bits(serdes, bu18rl82_gpio_id_high[offset].reg,
					bu18rl82_gpio_id_high[offset].mask,
					FIELD_PREP(GENMASK(2, 0),
					((fdata->gpio_id >> 8) & 0x7)));
			serdes_set_bits(serdes, bu18rl82_gpio_pden[offset].reg,
					bu18rl82_gpio_pden[offset].mask,
					FIELD_PREP(BIT(4), 0));
		}
	}

	return 0;
}

static struct serdes_chip_pinctrl_ops bu18rl82_pinctrl_ops = {
	.pinconf_set = bu18rl82_pinctrl_config_set,
	.pinmux_set = bu18rl82_pinctrl_set_pin_mux,
	.pinmux_group_set = bu18rl82_pinctrl_set_grp_mux,
};

static int bu18rl82_gpio_direction_input(struct serdes *serdes,
					 int gpio)
{
	return 0;
}

static int bu18rl82_gpio_direction_output(struct serdes *serdes,
					  int gpio, int value)
{
	return 0;
}

static int bu18rl82_gpio_get_level(struct serdes *serdes, int gpio)
{
	return 0;
}

int bu18rl82_gpio_set_level(struct serdes *serdes,
			    int gpio, int value)
{
	return 0;
}

static int bu18rl82_gpio_set_config(struct serdes *serdes,
				    int gpio, unsigned long config)
{
	return 0;
}

static int bu18rl82_gpio_to_irq(struct serdes *serdes, int gpio)
{
	return 0;
}

static struct serdes_chip_gpio_ops bu18rl82_gpio_ops = {
	.direction_input = bu18rl82_gpio_direction_input,
	.direction_output = bu18rl82_gpio_direction_output,
	.get_level = bu18rl82_gpio_get_level,
	.set_level = bu18rl82_gpio_set_level,
	.set_config = bu18rl82_gpio_set_config,
	.to_irq = bu18rl82_gpio_to_irq,
};

struct serdes_chip_data serdes_bu18rl82_data = {
	.name		= "bu18rl82",
	.serdes_type	= TYPE_DES,
	.serdes_id	= ROHM_ID_BU18RL82,
	.pinctrl_info	= &bu18rl82_pinctrl_info,
	.pinctrl_ops	= &bu18rl82_pinctrl_ops,
	.gpio_ops	= &bu18rl82_gpio_ops,
	.bridge_ops	= &bu18rl82_bridge_ops,
};
EXPORT_SYMBOL_GPL(serdes_bu18rl82_data);

MODULE_LICENSE("GPL");
