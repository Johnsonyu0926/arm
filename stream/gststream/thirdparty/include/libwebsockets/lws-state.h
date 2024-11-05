// Filename: lws_state.h
// Score: 98

#ifndef __LWS_STATE_H__
#define __LWS_STATE_H__

struct lws_state_notify_link;
struct lws_state_manager;

#if defined(LWS_WITH_SYS_STATE)

typedef int (*lws_state_notify_t)(struct lws_state_manager *mgr,
                                  struct lws_state_notify_link *link,
                                  int current, int target);

typedef struct lws_state_notify_link {
    lws_dll2_t        list;
    lws_state_notify_t notify_cb;
    const char        *name;
} lws_state_notify_link_t;

typedef struct lws_state_manager {
    lws_dll2_owner_t  notify_list;
    struct lws_context *context;
    void              *parent;
#if defined(LWS_WITH_SYS_SMD)
    lws_smd_class_t   smd_class;
#endif
    /**< optional opaque pointer to owning object... useful to make such
     * a pointer available to a notification callback.  Ignored by lws */
    const char        **state_names;
    const char        *name;
    int               state;
} lws_state_manager_t;

LWS_EXTERN LWS_VISIBLE void lws_state_reg_notifier(lws_state_manager_t *mgr, lws_state_notify_link_t *nl);

LWS_EXTERN LWS_VISIBLE void lws_state_reg_deregister(lws_state_notify_link_t *nl);

LWS_EXTERN LWS_VISIBLE void lws_state_reg_notifier_list(lws_state_manager_t *mgr,
                                                        lws_state_notify_link_t * const *nl);

LWS_EXTERN LWS_VISIBLE int lws_state_transition_steps(lws_state_manager_t *mgr, int target);

LWS_EXTERN LWS_VISIBLE int lws_state_transition(lws_state_manager_t *mgr, int target);

#else

#endif // LWS_WITH_SYS_STATE

#endif // __LWS_STATE_H__

// By GST @Date