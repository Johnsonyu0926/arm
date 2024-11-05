// Filename: lws_service.h
// Score: 99

#ifndef LWS_SERVICE_H
#define LWS_SERVICE_H

#include <libwebsockets.h>

LWS_VISIBLE LWS_EXTERN int lws_service(struct lws_context *context, int timeout_ms);

LWS_VISIBLE LWS_EXTERN int lws_service_tsi(struct lws_context *context, int timeout_ms, int tsi);

LWS_VISIBLE LWS_EXTERN void lws_cancel_service_pt(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN void lws_cancel_service(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN int lws_service_fd(struct lws_context *context, struct lws_pollfd *pollfd);

LWS_VISIBLE LWS_EXTERN int lws_service_fd_tsi(struct lws_context *context, struct lws_pollfd *pollfd, int tsi);

LWS_VISIBLE LWS_EXTERN int lws_service_adjust_timeout(struct lws_context *context, int timeout_ms, int tsi);

#define lws_plat_service_tsi lws_service_tsi

LWS_VISIBLE LWS_EXTERN int lws_handle_POLLOUT_event(struct lws *wsi, struct lws_pollfd *pollfd);

#if defined(LWS_WITH_LIBUV) && defined(UV_ERRNO_MAP)
LWS_VISIBLE LWS_EXTERN uv_loop_t *lws_uv_getloop(struct lws_context *context, int tsi);

LWS_VISIBLE LWS_EXTERN void lws_libuv_static_refcount_add(uv_handle_t *handle, struct lws_context *context, int tsi);

LWS_VISIBLE LWS_EXTERN void lws_libuv_static_refcount_del(uv_handle_t *handle);
#endif

#if defined(LWS_PLAT_FREERTOS)
#define lws_libuv_static_refcount_add(_a, _b, _c)
#define lws_libuv_static_refcount_del NULL
#endif

#endif // LWS_SERVICE_H

// By GST @Date