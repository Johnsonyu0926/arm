// Filename: acpi.h
#ifndef ZEPHYR_INCLUDE_DRIVERS_ACPI_H_
#define ZEPHYR_INCLUDE_DRIVERS_ACPI_H_

#include <acpica/source/include/acpi.h>
#include <zephyr/drivers/pcie/pcie.h>

#define ACPI_RES_INVALID ACPI_RESOURCE_TYPE_MAX

#define ACPI_DRHD_FLAG_INCLUDE_PCI_ALL BIT(0)
#define ACPI_DMAR_FLAG_INTR_REMAP BIT(0)
#define ACPI_DMAR_FLAG_X2APIC_OPT_OUT BIT(1)
#define ACPI_DMAR_FLAG_DMA_CTRL_PLATFORM_OPT_IN BIT(2)

#define ACPI_MMIO_GET(res) (res)->reg_base[0].mmio
#define ACPI_IO_GET(res) (res)->reg_base[0].port
#define ACPI_RESOURCE_SIZE_GET(res) (res)->reg_base[0].length
#define ACPI_RESOURCE_TYPE_GET(res) (res)->reg_base[0].type

#define ACPI_MULTI_MMIO_GET(res, idx) (res)->reg_base[idx].mmio
#define ACPI_MULTI_IO_GET(res, idx) (res)->reg_base[idx].port
#define ACPI_MULTI_RESOURCE_SIZE_GET(res, idx) (res)->reg_base[idx].length
#define ACPI_MULTI_RESOURCE_TYPE_GET(res, idx) (res)->reg_base[idx].type

#define ACPI_RESOURCE_COUNT_GET(res) (res)->mmio_max

enum acpi_res_type {
	ACPI_RES_TYPE_IO,
	ACPI_RES_TYPE_MEM,
	ACPI_RES_TYPE_UNKNOWN,
};

struct acpi_dev {
	ACPI_HANDLE handle;
	char *path;
	ACPI_RESOURCE *res_lst;
	int res_type;
	ACPI_DEVICE_INFO *dev_info;
};

union acpi_dmar_id {
	struct {
		uint16_t function : 3;
		uint16_t device : 5;
		uint16_t bus : 8;
	} bits;
	uint16_t raw;
};

struct acpi_mcfg {
	ACPI_TABLE_HEADER header;
	uint64_t _reserved;
	ACPI_MCFG_ALLOCATION pci_segs[];
} __packed;

struct acpi_irq_resource {
	uint32_t flags;
	uint8_t irq_vector_max;
	uint16_t *irqs;
};

struct acpi_reg_base {
	enum acpi_res_type type;
	union {
		uintptr_t mmio;
		uintptr_t port;
	};
	uint32_t length;
};

struct acpi_mmio_resource {
	uint8_t mmio_max;
	struct acpi_reg_base *reg_base;
};

#define ACPI_DT_HID(node_id) DT_PROP(node_id, acpi_hid)
#define ACPI_DT_UID(node_id) DT_PROP_OR(node_id, acpi_uid, NULL)
#define ACPI_DT_HAS_HID(node_id) DT_NODE_HAS_PROP(node_id, acpi_hid)
#define ACPI_DT_HAS_UID(node_id) DT_NODE_HAS_PROP(node_id, acpi_uid)

int acpi_legacy_irq_init(const char *hid, const char *uid);
uint32_t acpi_legacy_irq_get(pcie_bdf_t bdf);
int acpi_current_resource_get(char *dev_name, ACPI_RESOURCE **res);
int acpi_possible_resource_get(char *dev_name, ACPI_RESOURCE **res);
int acpi_current_resource_free(ACPI_RESOURCE *res);
ACPI_RESOURCE *acpi_resource_parse(ACPI_RESOURCE *res, int res_type);
struct acpi_dev *acpi_device_get(const char *hid, const char *uid);
struct acpi_dev *acpi_device_by_index_get(int index);

static inline ACPI_RESOURCE_IRQ *acpi_irq_res_get(ACPI_RESOURCE *res_lst) {
	ACPI_RESOURCE *res = acpi_resource_parse(res_lst, ACPI_RESOURCE_TYPE_IRQ);
	return res ? &res->Data.Irq : NULL;
}

int acpi_device_irq_get(struct acpi_dev *child_dev, struct acpi_irq_resource *irq_res);
int acpi_device_mmio_get(struct acpi_dev *child_dev, struct acpi_mmio_resource *mmio_res);
int acpi_device_type_get(ACPI_RESOURCE *res);
void *acpi_table_get(char *signature, int inst);
int acpi_madt_entry_get(int type, ACPI_SUBTABLE_HEADER **tables, int *num_inst);
int acpi_dmar_entry_get(enum AcpiDmarType type, ACPI_SUBTABLE_HEADER **tables);
int acpi_drhd_get(enum AcpiDmarScopeType scope, ACPI_DMAR_DEVICE_SCOPE *dev_scope,
		  union acpi_dmar_id *dmar_id, int *num_inst, int max_inst);

typedef void (*dmar_foreach_subtable_func_t)(ACPI_DMAR_HEADER *subtable, void *arg);
typedef void (*dmar_foreach_devscope_func_t)(ACPI_DMAR_DEVICE_SCOPE *devscope, void *arg);

void acpi_dmar_foreach_subtable(ACPI_TABLE_DMAR *dmar, dmar_foreach_subtable_func_t func,
				void *arg);
void acpi_dmar_foreach_devscope(ACPI_DMAR_HARDWARE_UNIT *hu,
				dmar_foreach_devscope_func_t func, void *arg);
int acpi_dmar_ioapic_get(uint16_t *ioapic_id);
ACPI_MADT_LOCAL_APIC *acpi_local_apic_get(int cpu_num);
int acpi_invoke_method(char *path, ACPI_OBJECT_LIST *arg_list, ACPI_OBJECT *ret_obj);

#endif /* ZEPHYR_INCLUDE_DRIVERS_ACPI_H_ */

// By GST @Date