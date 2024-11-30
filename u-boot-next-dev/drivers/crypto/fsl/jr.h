// jr.h

#ifndef __JR_H
#define __JR_H

#include <linux/compiler.h>

#define JR_SIZE 4
/* Timeout currently defined as 90 sec */
#define CONFIG_SEC_DEQ_TIMEOUT	90000000U

#define DEFAULT_JR_ID		0
#define DEFAULT_JR_LIODN	0
#define DEFAULT_IRQ		0	/* Interrupts not to be configured */

#define MCFGR_SWRST       ((uint32_t)(1)<<31) /* Software Reset */
#define MCFGR_DMA_RST     ((uint32_t)(1)<<28) /* DMA Reset */
#define MCFGR_PS_SHIFT          16
#define MCFGR_AWCACHE_SHIFT	8
#define MCFGR_AWCACHE_MASK	(0xf << MCFGR_AWCACHE_SHIFT)
#define MCFGR_ARCACHE_SHIFT	12
#define MCFGR_ARCACHE_MASK	(0xf << MCFGR_ARCACHE_SHIFT)

#define JR_INTMASK	  0x00000001
#define JRCR_RESET                  0x01
#define JRINT_ERR_HALT_INPROGRESS   0x4
#define JRINT_ERR_HALT_MASK         0xc
#define JRNSLIODN_SHIFT		16
#define JRNSLIODN_MASK		0x0fff0000
#define JRSLIODN_SHIFT		0
#define JRSLIODN_MASK		0x00000fff

#define JQ_DEQ_ERR		-1
#define JQ_DEQ_TO_ERR		-2
#define JQ_ENQ_ERR		-3

/**
 * @brief Structure representing an operation ring
 */
struct op_ring {
	phys_addr_t desc;
	uint32_t status;
} __packed;

/**
 * @brief Structure representing job ring information
 */
struct jr_info {
	void (*callback)(uint32_t status, void *arg);
	phys_addr_t desc_phys_addr;
	uint32_t desc_len;
	uint32_t op_done;
	void *arg;
};

/**
 * @brief Structure representing a job ring
 */
struct jobring {
	int jq_id;
	int irq;
	int liodn;
	int head;        /* Index where software would enqueue the descriptor in the input ring */
	int tail;        /* Index used by software while enqueuing to determine if there is any space left in the input rings */
	int read_idx;    /* Read index of the output ring */
	int write_idx;   /* Write index to input ring */
	int size;        /* Size of the rings */
	int op_size;     /* Operation ring size aligned to cache line size */
	dma_addr_t *input_ring;  /* Circular ring of input descriptors */
	struct jr_info info[JR_SIZE];  /* Circular ring containing info regarding descriptors in input and output ring */
	struct op_ring *output_ring;  /* Circular ring of output descriptors */
	uint32_t sec_offset;  /* Offset in CCSR to the SEC engine to which this JR belongs */
};

/**
 * @brief Structure representing the result of an operation
 */
struct result {
	int done;
	uint32_t status;
};

/**
 * @brief Print the status of the CAAM job ring
 *
 * @param status Status code
 */
void caam_jr_strstatus(u32 status);

/**
 * @brief Run a descriptor on the job ring
 *
 * @param desc Pointer to the descriptor
 * @return 0 on success, negative error code on failure
 */
int run_descriptor_jr(uint32_t *desc);

#endif /* __JR_H */
//GST