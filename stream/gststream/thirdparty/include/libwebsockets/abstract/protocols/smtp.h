// Score: 97

#ifndef __LWS_SMTP_H__
#define __LWS_SMTP_H__

#include <time.h>
#include "lws_vhost.h"

#if defined(LWS_WITH_SMTP)

enum {
    LTMI_PSMTP_V_HELO = LTMI_PROTOCOL_BASE,
    LTMI_PSMTP_V_LWS_SMTP_EMAIL_T,
};

enum {
    LWS_SMTP_DISPOSITION_SENT,
    LWS_SMTP_DISPOSITION_FAILED,
    LWS_SMTP_DISPOSITION_FAILED_DESTROY
};

typedef struct lws_smtp_sequencer_args {
    const char helo[32];
    struct lws_vhost *vhost;
    time_t retry_interval;
    time_t delivery_timeout;
    size_t email_queue_max;
    size_t max_content_size;
} lws_smtp_sequencer_args_t;

typedef struct lws_smtp_sequencer lws_smtp_sequencer_t;
typedef struct lws_smtp_email lws_smtp_email_t;

LWS_VISIBLE LWS_EXTERN lws_smtp_sequencer_t *lws_smtp_sequencer_create(const lws_smtp_sequencer_args_t *args);

LWS_VISIBLE LWS_EXTERN void lws_smtp_sequencer_destroy(lws_smtp_sequencer_t *s);

typedef int (*lws_smtp_cb_t)(void *e, void *d, int disp, const void *b, size_t l);

LWS_VISIBLE LWS_EXTERN int lws_smtpc_add_email(lws_smtp_sequencer_t *s, const char *payload, size_t payload_len, const char *sender, const char *recipient, void *data, lws_smtp_cb_t done);

LWS_VISIBLE LWS_EXTERN int lws_smtpc_free_email(lws_smtp_email_t *e);

#endif // LWS_WITH_SMTP

#endif // __LWS_SMTP_H__

// By GST @Date