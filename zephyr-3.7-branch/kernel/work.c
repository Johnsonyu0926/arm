//kernel/work.c
#include <zephyr/kernel.h>
#include <zephyr/kernel_structs.h>
#include <wait_q.h>
#include <zephyr/spinlock.h>
#include <errno.h>
#include <ksched.h>
#include <zephyr/sys/printk.h>

static inline void flag_clear(uint32_t *flagp, uint32_t bit)
{
	*flagp &= ~BIT(bit);
}

static inline void flag_set(uint32_t *flagp, uint32_t bit)
{
	*flagp |= BIT(bit);
}

static inline bool flag_test(const uint32_t *flagp, uint32_t bit)
{
	return (*flagp & BIT(bit)) != 0U;
}

static inline bool flag_test_and_clear(uint32_t *flagp, int bit)
{
	bool ret = flag_test(flagp, bit);
	flag_clear(flagp, bit);
	return ret;
}

static inline void flags_set(uint32_t *flagp, uint32_t flags)
{
	*flagp = flags;
}

static inline uint32_t flags_get(const uint32_t *flagp)
{
	return *flagp;
}

/* Lock to protect the internal state of all work items, work queues,
 * and pending_cancels.
 */
static struct k_spinlock lock;

/* Invoked by work thread */
static void handle_flush(struct k_work *work) { }

static inline void init_flusher(struct z_work_flusher *flusher)
{
	struct k_work *work = &flusher->work;
	k_sem_init(&flusher->sem, 0, 1);
	k_work_init(&flusher->work, handle_flush);
	flag_set(&work->flags, K_WORK_FLUSHING_BIT);
}

/* List of pending cancellations. */
static sys_slist_t pending_cancels;

static inline void init_work_cancel(struct z_work_canceller *canceler, struct k_work *work)
{
	k_sem_init(&canceler->sem, 0, 1);
	canceler->work = work;
	sys_slist_append(&pending_cancels, &canceler->node);
}

static void finalize_flush_locked(struct k_work *work)
{
	struct z_work_flusher *flusher = CONTAINER_OF(work, struct z_work_flusher, work);
	flag_clear(&work->flags, K_WORK_FLUSHING_BIT);
	k_sem_give(&flusher->sem);
}

static void finalize_cancel_locked(struct k_work *work)
{
	struct z_work_canceller *wc, *tmp;
	sys_snode_t *prev = NULL;

	flag_clear(&work->flags, K_WORK_CANCELING_BIT);

	SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&pending_cancels, wc, tmp, node) {
		if (wc->work == work) {
			sys_slist_remove(&pending_cancels, prev, &wc->node);
			k_sem_give(&wc->sem);
			break;
		}
		prev = &wc->node;
	}
}

void k_work_init(struct k_work *work, k_work_handler_t handler)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(handler != NULL);

	*work = (struct k_work)Z_WORK_INITIALIZER(handler);

	SYS_PORT_TRACING_OBJ_INIT(k_work, work);
}

static inline int work_busy_get_locked(const struct k_work *work)
{
	return flags_get(&work->flags) & K_WORK_MASK;
}

int k_work_busy_get(const struct k_work *work)
{
	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = work_busy_get_locked(work);
	k_spin_unlock(&lock, key);
	return ret;
}

static void queue_flusher_locked(struct k_work_q *queue, struct k_work *work, struct z_work_flusher *flusher)
{
	bool in_list = false;
	struct k_work *wn;

	SYS_SLIST_FOR_EACH_CONTAINER(&queue->pending, wn, node) {
		if (wn == work) {
			in_list = true;
			break;
		}
	}

	init_flusher(flusher);
	if (in_list) {
		sys_slist_insert(&queue->pending, &work->node, &flusher->work.node);
	} else {
		sys_slist_prepend(&queue->pending, &flusher->work.node);
	}
}

static inline void queue_remove_locked(struct k_work_q *queue, struct k_work *work)
{
	if (flag_test_and_clear(&work->flags, K_WORK_QUEUED_BIT)) {
		(void)sys_slist_find_and_remove(&queue->pending, &work->node);
	}
}

static inline bool notify_queue_locked(struct k_work_q *queue)
{
	bool rv = false;
	if (queue != NULL) {
		rv = z_sched_wake(&queue->notifyq, 0, NULL);
	}
	return rv;
}

static inline int queue_submit_locked(struct k_work_q *queue, struct k_work *work)
{
	if (queue == NULL) {
		return -EINVAL;
	}

	int ret;
	bool chained = (_current == &queue->thread) && !k_is_in_isr();
	bool draining = flag_test(&queue->flags, K_WORK_QUEUE_DRAIN_BIT);
	bool plugged = flag_test(&queue->flags, K_WORK_QUEUE_PLUGGED_BIT);

	if (!flag_test(&queue->flags, K_WORK_QUEUE_STARTED_BIT)) {
		ret = -ENODEV;
	} else if (draining && !chained) {
		ret = -EBUSY;
	} else if (plugged && !draining) {
		ret = -EBUSY;
	} else {
		sys_slist_append(&queue->pending, &work->node);
		ret = 1;
		(void)notify_queue_locked(queue);
	}

	return ret;
}

static int submit_to_queue_locked(struct k_work *work, struct k_work_q **queuep)
{
	int ret = 0;

	if (flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		ret = -EBUSY;
	} else if (!flag_test(&work->flags, K_WORK_QUEUED_BIT)) {
		ret = 1;

		if (*queuep == NULL) {
			*queuep = work->queue;
		}

		if (flag_test(&work->flags, K_WORK_RUNNING_BIT)) {
			__ASSERT_NO_MSG(work->queue != NULL);
			*queuep = work->queue;
			ret = 2;
		}

		int rc = queue_submit_locked(*queuep, work);

		if (rc < 0) {
			ret = rc;
		} else {
			flag_set(&work->flags, K_WORK_QUEUED_BIT);
			work->queue = *queuep;
		}
	} else {
		ret = 0;
	}

	if (ret <= 0) {
		*queuep = NULL;
	}

	return ret;
}

int z_work_submit_to_queue(struct k_work_q *queue, struct k_work *work)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(work->handler != NULL);

	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = submit_to_queue_locked(work, &queue);
	k_spin_unlock(&lock, key);

	return ret;
}

int k_work_submit_to_queue(struct k_work_q *queue, struct k_work *work)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, submit_to_queue, queue, work);

	int ret = z_work_submit_to_queue(queue, work);

	if (ret > 0) {
		z_reschedule_unlocked();
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, submit_to_queue, queue, work, ret);

	return ret;
}

int k_work_submit(struct k_work *work)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, submit, work);

	int ret = k_work_submit_to_queue(&k_sys_work_q, work);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, submit, work, ret);

	return ret;
}

static bool work_flush_locked(struct k_work *work, struct z_work_flusher *flusher)
{
	bool need_flush = (flags_get(&work->flags) & (K_WORK_QUEUED | K_WORK_RUNNING)) != 0U;

	if (need_flush) {
		struct k_work_q *queue = work->queue;
		__ASSERT_NO_MSG(queue != NULL);
		queue_flusher_locked(queue, work, flusher);
		notify_queue_locked(queue);
	}

	return need_flush;
}

bool k_work_flush(struct k_work *work, struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(!flag_test(&work->flags, K_WORK_DELAYABLE_BIT));
	__ASSERT_NO_MSG(!k_is_in_isr());
	__ASSERT_NO_MSG(sync != NULL);
#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(arch_mem_coherent(sync));
#endif /* CONFIG_KERNEL_COHERENCE */

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, flush, work);

	struct z_work_flusher *flusher = &sync->flusher;
	k_spinlock_key_t key = k_spin_lock(&lock);

	bool need_flush = work_flush_locked(work, flusher);

	k_spin_unlock(&lock, key);

	if (need_flush) {
		SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(k_work, flush, work, K_FOREVER);
		k_sem_take(&flusher->sem, K_FOREVER);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, flush, work, need_flush);

	return need_flush;
}

static int cancel_async_locked(struct k_work *work)
{
	if (!flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
		queue_remove_locked(work->queue, work);
	}

	int ret = work_busy_get_locked(work);

	if (ret != 0) {
		flag_set(&work->flags, K_WORK_CANCELING_BIT);
		ret = work_busy_get_locked(work);
	}

	return ret;
}

static bool cancel_sync_locked(struct k_work *work, struct z_work_canceller *canceller)
{
	bool ret = flag_test(&work->flags, K_WORK_CANCELING_BIT);

	if (ret) {
		init_work_cancel(canceller, work);
	}

	return ret;
}

int k_work_cancel(struct k_work *work)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(!flag_test(&work->flags, K_WORK_DELAYABLE_BIT));

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel, work);

	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = cancel_async_locked(work);
	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel, work, ret);

	return ret;
}

bool k_work_cancel_sync(struct k_work *work, struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(work != NULL);
	__ASSERT_NO_MSG(sync != NULL);
	__ASSERT_NO_MSG(!flag_test(&work->flags, K_WORK_DELAYABLE_BIT));
	__ASSERT_NO_MSG(!k_is_in_isr());
#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(arch_mem_coherent(sync));
#endif /* CONFIG_KERNEL_COHERENCE */

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel_sync, work, sync);

	struct z_work_canceller *canceller = &sync->canceller;
	k_spinlock_key_t key = k_spin_lock(&lock);
	bool pending = (work_busy_get_locked(work) != 0U);
	bool need_wait = false;

	if (pending) {
		(void)cancel_async_locked(work);
		need_wait = cancel_sync_locked(work, canceller);
	}

	k_spin_unlock(&lock, key);

	if (need_wait) {
		SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(k_work, cancel_sync, work, sync);
		k_sem_take(&canceller->sem, K_FOREVER);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel_sync, work, sync, pending);
	return pending;
}

static void work_queue_main(void *workq_ptr, void *p2, void *p3)
{
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	struct k_work_q *queue = (struct k_work_q *)workq_ptr;

	while (true) {
		sys_snode_t *node;
		struct k_work *work = NULL;
		k_work_handler_t handler = NULL;
		k_spinlock_key_t key = k_spin_lock(&lock);
		bool yield;

		node = sys_slist_get(&queue->pending);
		if (node != NULL) {
			flag_set(&queue->flags, K_WORK_QUEUE_BUSY_BIT);
			work = CONTAINER_OF(node, struct k_work, node);
			flag_set(&work->flags, K_WORK_RUNNING_BIT);
			flag_clear(&work->flags, K_WORK_QUEUED_BIT);
			handler = work->handler;
		} else if (flag_test_and_clear(&queue->flags, K_WORK_QUEUE_DRAIN_BIT)) {
			(void)z_sched_wake_all(&queue->drainq, 1, NULL);
		}

		if (work == NULL) {
			(void)z_sched_wait(&lock, key, &queue->notifyq, K_FOREVER, NULL);
			continue;
		}

		k_spin_unlock(&lock, key);

		__ASSERT_NO_MSG(handler != NULL);
		handler(work);

		key = k_spin_lock(&lock);

		flag_clear(&work->flags, K_WORK_RUNNING_BIT);
		if (flag_test(&work->flags, K_WORK_FLUSHING_BIT)) {
			finalize_flush_locked(work);
		}
		if (flag_test(&work->flags, K_WORK_CANCELING_BIT)) {
			finalize_cancel_locked(work);
		}

		flag_clear(&queue->flags, K_WORK_QUEUE_BUSY_BIT);
		yield = !flag_test(&queue->flags, K_WORK_QUEUE_NO_YIELD_BIT);
		k_spin_unlock(&lock, key);

		if (yield) {
			k_yield();
		}
	}
}

void k_work_queue_init(struct k_work_q *queue)
{
	__ASSERT_NO_MSG(queue != NULL);

	*queue = (struct k_work_q) {
		.flags = 0,
	};

	SYS_PORT_TRACING_OBJ_INIT(k_work_queue, queue);
}

void k_work_queue_start(struct k_work_q *queue, k_thread_stack_t *stack, size_t stack_size, int prio, const struct k_work_queue_config *cfg)
{
	__ASSERT_NO_MSG(queue);
	__ASSERT_NO_MSG(stack);
	__ASSERT_NO_MSG(!flag_test(&queue->flags, K_WORK_QUEUE_STARTED_BIT));
	uint32_t flags = K_WORK_QUEUE_STARTED;

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work_queue, start, queue);

	sys_slist_init(&queue->pending);
	z_waitq_init(&queue->notifyq);
	z_waitq_init(&queue->drainq);

	if ((cfg != NULL) && cfg->no_yield) {
		flags |= K_WORK_QUEUE_NO_YIELD;
	}

	flags_set(&queue->flags, flags);

	(void)k_thread_create(&queue->thread, stack, stack_size, work_queue_main, queue, NULL, NULL, prio, 0, K_FOREVER);

	if ((cfg != NULL) && (cfg->name != NULL)) {
		k_thread_name_set(&queue->thread, cfg->name);
	}

	if ((cfg != NULL) && (cfg->essential)) {
		queue->thread.base.user_options |= K_ESSENTIAL;
	}

	k_thread_start(&queue->thread);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work_queue, start, queue);
}

int k_work_queue_drain(struct k_work_q *queue, bool plug)
{
	__ASSERT_NO_MSG(queue);
	__ASSERT_NO_MSG(!k_is_in_isr());

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work_queue, drain, queue);

	int ret = 0;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (((flags_get(&queue->flags) & (K_WORK_QUEUE_BUSY | K_WORK_QUEUE_DRAIN)) != 0U) || plug || !sys_slist_is_empty(&queue->pending)) {
		flag_set(&queue->flags, K_WORK_QUEUE_DRAIN_BIT);
		if (plug) {
			flag_set(&queue->flags, K_WORK_QUEUE_PLUGGED_BIT);
		}

		notify_queue_locked(queue);
		ret = z_sched_wait(&lock, key, &queue->drainq, K_FOREVER, NULL);
	} else {
		k_spin_unlock(&lock, key);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work_queue, drain, queue, ret);

	return ret;
}

int k_work_queue_unplug(struct k_work_q *queue)
{
	__ASSERT_NO_MSG(queue);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work_queue, unplug, queue);

	int ret = -EALREADY;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (flag_test_and_clear(&queue->flags, K_WORK_QUEUE_PLUGGED_BIT)) {
		ret = 0;
	}

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work_queue, unplug, queue, ret);

	return ret;
}

#ifdef CONFIG_SYS_CLOCK_EXISTS

static void work_timeout(struct _timeout *to)
{
	struct k_work_delayable *dw = CONTAINER_OF(to, struct k_work_delayable, timeout);
	struct k_work *wp = &dw->work;
	k_spinlock_key_t key = k_spin_lock(&lock);
	struct k_work_q *queue = NULL;

	if (flag_test_and_clear(&wp->flags, K_WORK_DELAYED_BIT)) {
		queue = dw->queue;
		(void)submit_to_queue_locked(wp, &queue);
	}

	k_spin_unlock(&lock, key);
}

void k_work_init_delayable(struct k_work_delayable *dwork, k_work_handler_t handler)
{
	__ASSERT_NO_MSG(dwork != NULL);
	__ASSERT_NO_MSG(handler != NULL);

	*dwork = (struct k_work_delayable){
		.work = {
			.handler = handler,
			.flags = K_WORK_DELAYABLE,
		},
	};
	z_init_timeout(&dwork->timeout);

	SYS_PORT_TRACING_OBJ_INIT(k_work_delayable, dwork);
}

static inline int work_delayable_busy_get_locked(const struct k_work_delayable *dwork)
{
	return flags_get(&dwork->work.flags) & K_WORK_MASK;
}

int k_work_delayable_busy_get(const struct k_work_delayable *dwork)
{
	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = work_delayable_busy_get_locked(dwork);
	k_spin_unlock(&lock, key);
	return ret;
}

static int schedule_for_queue_locked(struct k_work_q **queuep, struct k_work_delayable *dwork, k_timeout_t delay)
{
	int ret = 1;
	struct k_work *work = &dwork->work;

	if (K_TIMEOUT_EQ(delay, K_NO_WAIT)) {
		return submit_to_queue_locked(work, queuep);
	}

	flag_set(&work->flags, K_WORK_DELAYED_BIT);
	dwork->queue = *queuep;

	z_add_timeout(&dwork->timeout, work_timeout, delay);

	return ret;
}

static inline bool unschedule_locked(struct k_work_delayable *dwork)
{
	bool ret = false;
	struct k_work *work = &dwork->work;

	if (flag_test_and_clear(&work->flags, K_WORK_DELAYED_BIT)) {
		ret = z_abort_timeout(&dwork->timeout) == 0;
	}

	return ret;
}

static int cancel_delayable_async_locked(struct k_work_delayable *dwork)
{
	(void)unschedule_locked(dwork);
	return cancel_async_locked(&dwork->work);
}

int k_work_schedule_for_queue(struct k_work_q *queue, struct k_work_delayable *dwork, k_timeout_t delay)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, schedule_for_queue, queue, dwork, delay);

	struct k_work *work = &dwork->work;
	int ret = 0;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if ((work_busy_get_locked(work) & ~K_WORK_RUNNING) == 0U) {
		ret = schedule_for_queue_locked(&queue, dwork, delay);
	}

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, schedule_for_queue, queue, dwork, delay, ret);

	return ret;
}

int k_work_schedule(struct k_work_delayable *dwork, k_timeout_t delay)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, schedule, dwork, delay);

	int ret = k_work_schedule_for_queue(&k_sys_work_q, dwork, delay);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, schedule, dwork, delay, ret);

	return ret;
}

int k_work_reschedule_for_queue(struct k_work_q *queue, struct k_work_delayable *dwork, k_timeout_t delay)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, reschedule_for_queue, queue, dwork, delay);

	int ret;
	k_spinlock_key_t key = k_spin_lock(&lock);

	(void)unschedule_locked(dwork);

	ret = schedule_for_queue_locked(&queue, dwork, delay);

	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, reschedule_for_queue, queue, dwork, delay, ret);

	return ret;
}

int k_work_reschedule(struct k_work_delayable *dwork, k_timeout_t delay)
{
	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, reschedule, dwork, delay);

	int ret = k_work_reschedule_for_queue(&k_sys_work_q, dwork, delay);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, reschedule, dwork, delay, ret);

	return ret;
}

int k_work_cancel_delayable(struct k_work_delayable *dwork)
{
	__ASSERT_NO_MSG(dwork != NULL);

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel_delayable, dwork);

	k_spinlock_key_t key = k_spin_lock(&lock);
	int ret = cancel_delayable_async_locked(dwork);
	k_spin_unlock(&lock, key);

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel_delayable, dwork, ret);

	return ret;
}

bool k_work_cancel_delayable_sync(struct k_work_delayable *dwork, struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(dwork != NULL);
	__ASSERT_NO_MSG(sync != NULL);
	__ASSERT_NO_MSG(!k_is_in_isr());
#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(arch_mem_coherent(sync));
#endif /* CONFIG_KERNEL_COHERENCE */

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, cancel_delayable_sync, dwork, sync);

	struct z_work_canceller *canceller = &sync->canceller;
	k_spinlock_key_t key = k_spin_lock(&lock);
	bool pending = (work_delayable_busy_get_locked(dwork) != 0U);
	bool need_wait = false;

	if (pending) {
		(void)cancel_delayable_async_locked(dwork);
		need_wait = cancel_sync_locked(&dwork->work, canceller);
	}

	k_spin_unlock(&lock, key);

	if (need_wait) {
		k_sem_take(&canceller->sem, K_FOREVER);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, cancel_delayable_sync, dwork, sync, pending);
	return pending;
}

bool k_work_flush_delayable(struct k_work_delayable *dwork, struct k_work_sync *sync)
{
	__ASSERT_NO_MSG(dwork != NULL);
	__ASSERT_NO_MSG(sync != NULL);
	__ASSERT_NO_MSG(!k_is_in_isr());
#ifdef CONFIG_KERNEL_COHERENCE
	__ASSERT_NO_MSG(arch_mem_coherent(sync));
#endif /* CONFIG_KERNEL_COHERENCE */

	SYS_PORT_TRACING_OBJ_FUNC_ENTER(k_work, flush_delayable, dwork, sync);

	struct k_work *work = &dwork->work;
	struct z_work_flusher *flusher = &sync->flusher;
	k_spinlock_key_t key = k_spin_lock(&lock);

	if (work_busy_get_locked(work) == 0U) {
		k_spin_unlock(&lock, key);

		SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, flush_delayable, dwork, sync, false);

		return false;
	}

	if (unschedule_locked(dwork)) {
		struct k_work_q *queue = dwork->queue;
		(void)submit_to_queue_locked(work, &queue);
	}

	bool need_flush = work_flush_locked(work, flusher);

	k_spin_unlock(&lock, key);

	if (need_flush) {
		k_sem_take(&flusher->sem, K_FOREVER);
	}

	SYS_PORT_TRACING_OBJ_FUNC_EXIT(k_work, flush_delayable, dwork, sync, need_flush);

	return need_flush;
}

#endif /* CONFIG_SYS_CLOCK_EXISTS */
//GST