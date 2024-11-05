// Score: 98

#ifndef __LWS_THREADPOOL_H__
#define __LWS_THREADPOOL_H__

struct lws_threadpool;
struct lws_threadpool_task;

enum lws_threadpool_task_status {
    LWS_TP_STATUS_QUEUED,
    LWS_TP_STATUS_RUNNING,
    LWS_TP_STATUS_SYNCING,
    LWS_TP_STATUS_STOPPING,
    LWS_TP_STATUS_FINISHED,
    LWS_TP_STATUS_STOPPED,
};

enum lws_threadpool_task_return {
    LWS_TP_RETURN_CHECKING_IN,
    LWS_TP_RETURN_SYNC,
    LWS_TP_RETURN_FINISHED,
    LWS_TP_RETURN_STOPPED,
    LWS_TP_RETURN_FLAG_OUTLIVE = 64
};

struct lws_threadpool_create_args {
    int threads;
    int max_queue_depth;
};

struct lws_threadpool_task_args {
#if defined(LWS_WITH_SECURE_STREAMS)
    struct lws_ss_handle *ss;
#endif
    struct lws *wsi;
    void *user;
    const char *name;
    char async_task;
    enum lws_threadpool_task_return (*task)(void *user, enum lws_threadpool_task_status s);
    void (*cleanup)(struct lws *wsi, void *user);
};

LWS_VISIBLE LWS_EXTERN struct lws_threadpool *lws_threadpool_create(struct lws_context *context, const struct lws_threadpool_create_args *args, const char *format, ...) LWS_FORMAT(3);

LWS_VISIBLE LWS_EXTERN void lws_threadpool_finish(struct lws_threadpool *tp);

LWS_VISIBLE LWS_EXTERN void lws_threadpool_destroy(struct lws_threadpool *tp);

LWS_VISIBLE LWS_EXTERN struct lws_threadpool_task *lws_threadpool_enqueue(struct lws_threadpool *tp, const struct lws_threadpool_task_args *args, const char *format, ...) LWS_FORMAT(3);

LWS_VISIBLE LWS_EXTERN int lws_threadpool_dequeue(struct lws *wsi) LWS_WARN_DEPRECATED;

LWS_VISIBLE LWS_EXTERN int lws_threadpool_dequeue_task(struct lws_threadpool_task *task);

LWS_VISIBLE LWS_EXTERN enum lws_threadpool_task_status lws_threadpool_task_status_wsi(struct lws *wsi, struct lws_threadpool_task **task, void **user) LWS_WARN_DEPRECATED;

LWS_VISIBLE LWS_EXTERN enum lws_threadpool_task_status lws_threadpool_task_status(struct lws_threadpool_task *task, void **user);

LWS_VISIBLE LWS_EXTERN enum lws_threadpool_task_status lws_threadpool_task_status_noreap(struct lws_threadpool_task *task);

LWS_VISIBLE LWS_EXTERN void lws_threadpool_task_sync(struct lws_threadpool_task *task, int stop);

LWS_VISIBLE LWS_EXTERN void lws_threadpool_dump(struct lws_threadpool *tp);

LWS_VISIBLE LWS_EXTERN struct lws_threadpool_task *lws_threadpool_get_task_wsi(struct lws *wsi);

#if defined(LWS_WITH_SECURE_STREAMS)
LWS_VISIBLE LWS_EXTERN struct lws_threadpool_task *lws_threadpool_get_task_ss(struct lws_ss_handle *ss);
#endif

LWS_VISIBLE LWS_EXTERN int lws_threadpool_foreach_task_wsi(struct lws *wsi, void *user, int (*cb)(struct lws_threadpool_task *task, void *user));

#if defined(LWS_WITH_SECURE_STREAMS)
LWS_VISIBLE LWS_EXTERN int lws_threadpool_foreach_task_ss(struct lws_ss_handle *ss, void *user, int (*cb)(struct lws_threadpool_task *task, void *user));
#endif

#endif // __LWS_THREADPOOL_H__

// By GST @Date