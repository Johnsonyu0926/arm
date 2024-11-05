// Score: 96

#ifndef __LWS_CALLBACK_H__
#define __LWS_CALLBACK_H__

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable_all_protocol(const struct lws_context *context, const struct lws_protocols *protocol);

LWS_VISIBLE LWS_EXTERN int lws_callback_on_writable_all_protocol_vhost(const struct lws_vhost *vhost, const struct lws_protocols *protocol);

LWS_VISIBLE LWS_EXTERN int lws_callback_all_protocol(struct lws_context *context, const struct lws_protocols *protocol, int reason);

LWS_VISIBLE LWS_EXTERN int lws_callback_all_protocol_vhost(struct lws_vhost *vh, const struct lws_protocols *protocol, int reason) LWS_WARN_DEPRECATED;

LWS_VISIBLE int lws_callback_all_protocol_vhost_args(struct lws_vhost *vh, const struct lws_protocols *protocol, int reason, void *argp, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_callback_vhost_protocols(struct lws *wsi, int reason, void *in, size_t len) LWS_WARN_DEPRECATED;

LWS_VISIBLE LWS_EXTERN int lws_callback_vhost_protocols_vhost(struct lws_vhost *vh, int reason, void *in, size_t len);

LWS_VISIBLE LWS_EXTERN int lws_callback_http_dummy(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

LWS_VISIBLE LWS_EXTERN lws_sockfd_type lws_get_socket_fd(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN lws_fileofs_t lws_get_peer_write_allowance(struct lws *wsi);

#define LWSTXCR_US_TO_PEER 0
#define LWSTXCR_PEER_TO_US 1

LWS_VISIBLE LWS_EXTERN int lws_wsi_tx_credit(struct lws *wsi, char peer_to_us, int add);

#endif // __LWS_CALLBACK_H__

// By GST @Date