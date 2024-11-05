typedef uint64_t u_mt_t;

enum {
	LWSMTFL_REPORT_OUTLIERS				= (1 << 0),
	LWSMTFL_REPORT_OOB				= (1 << 1),
	LWSMTFL_REPORT_INACTIVITY_AT_PERIODIC		= (1 << 2),
	LWSMTFL_REPORT_MEAN				= (1 << 3),
	LWSMTFL_REPORT_ONLY_GO				= (1 << 4),
	LWSMTFL_REPORT_DUTY_WALLCLOCK_US		= (1 << 5),
	LWSMTFL_REPORT_HIST				= (1 << 6),
};

typedef struct lws_metrics_tag {
	lws_dll2_t	list;
	const char	*name;
} lws_metrics_tag_t;

LWS_EXTERN LWS_VISIBLE int lws_metrics_tag_add(lws_dll2_owner_t *owner, const char *name, const char *val);

#if defined(LWS_WITH_SYS_METRICS)
LWS_EXTERN LWS_VISIBLE int lws_metrics_tag_wsi_add(struct lws *wsi, const char *name, const char *val);
#else
#define lws_metrics_tag_wsi_add(_a, _b, _c)
#endif

#if defined(LWS_WITH_SECURE_STREAMS)
#if defined(LWS_WITH_SYS_METRICS)
LWS_EXTERN LWS_VISIBLE int lws_metrics_tag_ss_add(struct lws_ss_handle *ss, const char *name, const char *val);
#else
#define lws_metrics_tag_ss_add(_a, _b, _c)
#endif
#endif

LWS_EXTERN LWS_VISIBLE void lws_metrics_tags_destroy(lws_dll2_owner_t *owner);
LWS_EXTERN LWS_VISIBLE size_t lws_metrics_tags_serialize(lws_dll2_owner_t *owner, char *buf, size_t len);
LWS_EXTERN LWS_VISIBLE const char * lws_metrics_tag_get(lws_dll2_owner_t *owner, const char *name);

typedef struct lws_metric_bucket {
	struct lws_metric_bucket	*next;
	uint64_t			count;
} lws_metric_bucket_t;

#define lws_metric_bucket_name_len(_b) (*((uint8_t *)&(_b)[1]))
#define lws_metric_bucket_name(_b) (((const char *)&(_b)[1]) + 1)

typedef struct lws_metric_pub {
	const char		*name;
	void			*backend_opaque;
	lws_usec_t		us_first;
	lws_usec_t		us_last;
	lws_usec_t		us_dumped;

	union {
		struct {
			u_mt_t			sum[2];
			u_mt_t			min;
			u_mt_t			max;
			uint32_t		count[2];
		} agg;

		struct {
			lws_metric_bucket_t	*head;
			uint64_t		total_count;
			uint32_t		list_size;
		} hist;
	} u;

	uint8_t			flags;

} lws_metric_pub_t;

LWS_EXTERN LWS_VISIBLE void lws_metrics_hist_bump_priv_tagged(lws_metric_pub_t *mt, lws_dll2_owner_t *tow, lws_dll2_owner_t *tow2);

struct lws_metric;

typedef struct lws_metric_caliper {
	struct lws_dll2_owner	mtags_owner;
	struct lws_metric	*mt;
	lws_usec_t		us_start;
} lws_metric_caliper_t;

#if defined(LWS_WITH_SYS_METRICS)
#define lws_metrics_caliper_compose(_name) lws_metric_caliper_t _name;
#define lws_metrics_caliper_bind(_name, _mt) { if (_name.mt) { lwsl_err("caliper: overwrite %s\n", lws_metrics_priv_to_pub(_name.mt)->name); assert(0); } _name.mt = _mt; _name.us_start = lws_now_usecs(); }
#define lws_metrics_caliper_declare(_name, _mt) lws_metric_caliper_t _name = { .mt = _mt, .us_start = lws_now_usecs() }
#define lws_metrics_caliper_report(_name, _go_nogo) { if (_name.us_start) { lws_metric_event(_name.mt, _go_nogo, (u_mt_t)(lws_now_usecs() - _name.us_start)); } lws_metrics_caliper_done(_name); }
#define lws_metrics_caliper_report_hist(_name, pwsi) if (_name.mt) { lws_metrics_hist_bump_priv_tagged(lws_metrics_priv_to_pub(_name.mt), &_name.mtags_owner, pwsi ? &((pwsi)->cal_conn.mtags_owner) : NULL); lws_metrics_caliper_done(_name); }
#define lws_metrics_caliper_cancel(_name) { lws_metrics_caliper_done(_name); }
#define lws_metrics_hist_bump(_mt, _name) lws_metrics_hist_bump_(_mt, _name)
#define lws_metrics_hist_bump_priv(_mt, _name) lws_metrics_hist_bump_(lws_metrics_priv_to_pub(_mt), _name)
#define lws_metrics_caliper_done(_name) { _name.us_start = 0; _name.mt = NULL; lws_metrics_tags_destroy(&_name.mtags_owner); }
#else
#define lws_metrics_caliper_compose(_name)
#define lws_metrics_caliper_bind(_name, _mt)
#define lws_metrics_caliper_declare(_name, _mp)
#define lws_metrics_caliper_report(_name, _go_nogo)
#define lws_metrics_caliper_report_hist(_name, pwsiconn)
#define lws_metrics_caliper_cancel(_name)
#define lws_metrics_hist_bump(_mt, _name)
#define lws_metrics_hist_bump_priv(_mt, _name)
#define lws_metrics_caliper_done(_name)
#endif

LWS_EXTERN LWS_VISIBLE int lws_metrics_format(lws_metric_pub_t *pub, lws_metric_bucket_t **sub, char *buf, size_t len);
LWS_EXTERN LWS_VISIBLE int lws_metrics_hist_bump_(lws_metric_pub_t *pub, const char *name);
LWS_VISIBLE LWS_EXTERN int lws_metrics_foreach(struct lws_context *ctx, void *user, int (*cb)(lws_metric_pub_t *pub, void *user));
LWS_VISIBLE LWS_EXTERN int lws_metrics_hist_bump_describe_wsi(struct lws *wsi, lws_metric_pub_t *pub, const char *name);

enum {
	LMT_NORMAL = 0,
	LMT_OUTLIER,
	LMT_FAIL,
	LMT_COUNT,
};

typedef enum lws_metric_rpt {
	LMR_PERIODIC = 0,
	LMR_OUTLIER,
} lws_metric_rpt_kind_t;

#define METRES_GO	0
#define METRES_NOGO	1
// By GST @Date