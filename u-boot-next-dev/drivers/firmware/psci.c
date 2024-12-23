//u-boot-next-dev/drivers/firmware/psci.c
#include <common.h>
#include <dm.h>
#include <dm/lists.h>
#include <linux/libfdt.h>
#include <linux/arm-smccc.h>
#include <linux/errno.h>
#include <linux/psci.h>

psci_fn *invoke_psci_fn;

static unsigned long __invoke_psci_fn_hvc(unsigned long function_id,
			unsigned long arg0, unsigned long arg1,
			unsigned long arg2)
{
	struct arm_smccc_res res;

	arm_smccc_hvc(function_id, arg0, arg1, arg2, 0, 0, 0, 0, &res);
	return res.a0;
}

static unsigned long __invoke_psci_fn_smc(unsigned long function_id,
			unsigned long arg0, unsigned long arg1,
			unsigned long arg2)
{
	struct arm_smccc_res res;

	arm_smccc_smc(function_id, arg0, arg1, arg2, 0, 0, 0, 0, &res);
	return res.a0;
}

static int psci_bind(struct udevice *dev)
{
	/* No SYSTEM_RESET support for PSCI 0.1 */
	if (device_is_compatible(dev, "arm,psci-0.2") ||
	    device_is_compatible(dev, "arm,psci-1.0")) {
		int ret;

		/* bind psci-sysreset optionally */
		ret = device_bind_driver(dev, "psci-sysreset", "psci-sysreset",
					 NULL);
		if (ret)
			debug("PSCI System Reset was not bound.\n");
	}

	return 0;
}

static int psci_probe(struct udevice *dev)
{
	const char *method;

	method = dev_read_prop(dev, "method", NULL);
	if (!method) {
		printf("missing \"method\" property\n");
		return -ENXIO;
	}

	if (!strcmp("hvc", method)) {
		invoke_psci_fn = __invoke_psci_fn_hvc;
	} else if (!strcmp("smc", method)) {
		invoke_psci_fn = __invoke_psci_fn_smc;
	} else {
		printf("invalid \"method\" property: %s\n", method);
		return -EINVAL;
	}

	return 0;
}

static const struct udevice_id psci_of_match[] = {
	{ .compatible = "arm,psci" },
	{ .compatible = "arm,psci-0.2" },
	{ .compatible = "arm,psci-1.0" },
	{},
};

U_BOOT_DRIVER(psci) = {
	.name = "psci",
	.id = UCLASS_FIRMWARE,
	.of_match = psci_of_match,
	.bind = psci_bind,
	.probe = psci_probe,
};
