//kernel/system_work_q.c
#include <zephyr/kernel.h>
#include <zephyr/init.h>

/* Define the stack for the system work queue */
static K_KERNEL_STACK_DEFINE(sys_work_q_stack, CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE);

/* Declare the system work queue */
struct k_work_q k_sys_work_q;

/**
 * @brief Initialize the system work queue
 *
 * This function initializes and starts the system work queue.
 *
 * @return 0 on success
 */
static int k_sys_work_q_init(void)
{
	struct k_work_queue_config cfg = {
		.name = "sysworkq",
		.no_yield = IS_ENABLED(CONFIG_SYSTEM_WORKQUEUE_NO_YIELD),
		.essential = true,
	};

	k_work_queue_start(&k_sys_work_q,
			    sys_work_q_stack,
			    K_KERNEL_STACK_SIZEOF(sys_work_q_stack),
			    CONFIG_SYSTEM_WORKQUEUE_PRIORITY, &cfg);
	return 0;
}

/* Initialize the system work queue after the kernel has started */
SYS_INIT(k_sys_work_q_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
//GST