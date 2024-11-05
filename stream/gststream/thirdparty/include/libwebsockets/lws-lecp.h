#ifndef LECP_MAX_PARSING_STACK_DEPTH
#define LECP_MAX_PARSING_STACK_DEPTH	5
#endif
#ifndef LECP_MAX_DEPTH
#define LECP_MAX_DEPTH			12
#endif
#ifndef LECP_MAX_INDEX_DEPTH
#define LECP_MAX_INDEX_DEPTH		8
#endif
#ifndef LECP_MAX_PATH
#define LECP_MAX_PATH			128
#endif
#ifndef LECP_STRING_CHUNK
#define LECP_STRING_CHUNK		254
#endif

#define LECP_FLAG_CB_IS_VALUE 64

enum {
	LWS_CBOR_MAJTYP_UINT		= 0 << 5,
	LWS_CBOR_MAJTYP_INT_NEG		= 1 << 5,
	LWS_CBOR_MAJTYP_BSTR		= 2 << 5,
	LWS_CBOR_MAJTYP_TSTR		= 3 << 5,
	LWS_CBOR_MAJTYP_ARRAY		= 4 << 5,
	LWS_CBOR_MAJTYP_MAP		= 5 << 5,
	LWS_CBOR_MAJTYP_TAG		= 6 << 5,
	LWS_CBOR_MAJTYP_FLOAT		= 7 << 5,
	LWS_CBOR_MAJTYP_MASK		= 7 << 5,
	LWS_CBOR_1			= 24,
	LWS_CBOR_2			= 25,
	LWS_CBOR_4			= 26,
	LWS_CBOR_8			= 27,
	LWS_CBOR_RESERVED		= 28,
	LWS_CBOR_SUBMASK		= 0x1f,
	LWS_CBOR_SWK_FALSE		= 20,
	LWS_CBOR_SWK_TRUE		= 21,
	LWS_CBOR_SWK_NULL		= 22,
	LWS_CBOR_SWK_UNDEFINED		= 23,
	LWS_CBOR_M7_SUBTYP_SIMPLE_X8	= 24,
	LWS_CBOR_M7_SUBTYP_FLOAT16	= 25,
	LWS_CBOR_M7_SUBTYP_FLOAT32	= 26,
	LWS_CBOR_M7_SUBTYP_FLOAT64	= 27,
	LWS_CBOR_M7_BREAK		= 31,
	LWS_CBOR_INDETERMINITE		= 31,
	LWS_CBOR_WKTAG_DATETIME_STD	= 0,
	LWS_CBOR_WKTAG_DATETIME_EPOCH	= 1,
	LWS_CBOR_WKTAG_BIGNUM_UNSIGNED	= 2,
	LWS_CBOR_WKTAG_BIGNUM_NEGATIVE	= 3,
	LWS_CBOR_WKTAG_DECIMAL_FRAC	= 4,
	LWS_CBOR_WKTAG_BIGFLOAT		= 5,
	LWS_CBOR_WKTAG_COSE_ENC0	= 16,
	LWS_CBOR_WKTAG_COSE_MAC0	= 17,
	LWS_CBOR_WKTAG_COSE_SIGN1	= 18,
	LWS_CBOR_WKTAG_TO_B64U		= 21,
	LWS_CBOR_WKTAG_TO_B64		= 22,
	LWS_CBOR_WKTAG_TO_B16		= 23,
	LWS_CBOR_WKTAG_CBOR		= 24,
	LWS_CBOR_WKTAG_URI		= 32,
	LWS_CBOR_WKTAG_B64U		= 33,
	LWS_CBOR_WKTAG_B64		= 34,
	LWS_CBOR_WKTAG_MIME		= 36,
	LWS_CBOR_WKTAG_COSE_ENC		= 96,
	LWS_CBOR_WKTAG_COSE_MAC		= 97,
	LWS_CBOR_WKTAG_COSE_SIGN	= 98,
	LWS_CBOR_WKTAG_SELFDESCCBOR	= 55799
};

enum lecp_callbacks {
	LECPCB_CONSTRUCTED		= 0,
	LECPCB_DESTRUCTED		= 1,
	LECPCB_COMPLETE			= 3,
	LECPCB_FAILED			= 4,
	LECPCB_PAIR_NAME		= 5,
	LECPCB_VAL_TRUE			= LECP_FLAG_CB_IS_VALUE | 6,
	LECPCB_VAL_FALSE		= LECP_FLAG_CB_IS_VALUE | 7,
	LECPCB_VAL_NULL			= LECP_FLAG_CB_IS_VALUE | 8,
	LECPCB_VAL_NUM_INT		= LECP_FLAG_CB_IS_VALUE | 9,
	LECPCB_VAL_RESERVED		= LECP_FLAG_CB_IS_VALUE | 10,
	LECPCB_VAL_STR_START		= 11,
	LECPCB_VAL_STR_CHUNK		= LECP_FLAG_CB_IS_VALUE | 12,
	LECPCB_VAL_STR_END		= LECP_FLAG_CB_IS_VALUE | 13,
	LECPCB_ARRAY_START		= 14,
	LECPCB_ARRAY_END		= 15,
	LECPCB_OBJECT_START		= 16,
	LECPCB_OBJECT_END		= 17,
	LECPCB_TAG_START		= 18,
	LECPCB_TAG_END			= 19,
	LECPCB_VAL_NUM_UINT		= LECP_FLAG_CB_IS_VALUE | 20,
	LECPCB_VAL_UNDEFINED		= LECP_FLAG_CB_IS_VALUE | 21,
	LECPCB_VAL_FLOAT16		= LECP_FLAG_CB_IS_VALUE | 22,
	LECPCB_VAL_FLOAT32		= LECP_FLAG_CB_IS_VALUE | 23,
	LECPCB_VAL_FLOAT64		= LECP_FLAG_CB_IS_VALUE | 24,
	LECPCB_VAL_SIMPLE		= LECP_FLAG_CB_IS_VALUE | 25,
	LECPCB_VAL_BLOB_START		= 26,
	LECPCB_VAL_BLOB_CHUNK		= LECP_FLAG_CB_IS_VALUE | 27,
	LECPCB_VAL_BLOB_END		= LECP_FLAG_CB_IS_VALUE | 28,
	LECPCB_ARRAY_ITEM_START		= 29,
	LECPCB_ARRAY_ITEM_END		= 30,
	LECPCB_LITERAL_CBOR		= 31,
};

enum lecp_reasons {
	LECP_CONTINUE			= -1,
	LECP_REJECT_BAD_CODING		= -2,
	LECP_REJECT_UNKNOWN		= -3,
	LECP_REJECT_CALLBACK		= -4,
	LECP_STACK_OVERFLOW		= -5,
};

struct lecp_item {
	union {
		uint64_t	u64;
		int64_t		i64;
		uint64_t	u32;
		uint16_t	hf;
#if defined(LWS_WITH_CBOR_FLOAT)
		float		f;
		double		d;
#else
		uint32_t	f;
		uint64_t	d;
#endif
	} u;
	uint8_t			opcode;
};

struct lecp_ctx;
typedef signed char (*lecp_callback)(struct lecp_ctx *ctx, char reason);

struct _lecp_stack {
	char			s;
	uint8_t			p;
	char			i;
	char			indet;
	char			intermediate;
	char			pop_iss;
	uint64_t		tag;
	uint64_t		collect_rem;
	uint32_t		ordinal;
	uint8_t			opcode;
	uint8_t			send_new_array_item;
	uint8_t			barrier;
};

struct _lecp_parsing_stack {
	void			*user;
	lecp_callback		cb;
	const char * const	*paths;
	uint8_t			count_paths;
	uint8_t			ppos;
	uint8_t			path_match;
};

struct lecp_ctx {
	void *user;
	uint8_t			*collect_tgt;
	struct _lecp_parsing_stack pst[LECP_MAX_PARSING_STACK_DEPTH];
	struct _lecp_stack	st[LECP_MAX_DEPTH];
	uint16_t		i[LECP_MAX_INDEX_DEPTH];
	uint16_t		wild[LECP_MAX_INDEX_DEPTH];
	char			path[LECP_MAX_PATH];
	uint8_t			cbor[64];
	struct lecp_item	item;
	size_t			path_stride;
	size_t			used_in;
	uint16_t 		uni;
	uint8_t			npos;
	uint8_t			dcount;
	uint8_t			f;
	uint8_t			sp;
	uint8_t			ipos;
	uint8_t			count_paths;
	uint8_t			path_match;
	uint8_t			path_match_len;
	uint8_t			wildcount;
	uint8_t			pst_sp;
	uint8_t			outer_array;
	uint8_t			cbor_pos;
	uint8_t			literal_cbor_report;
	char			present;
	uint8_t			be;
	char buf[LECP_STRING_CHUNK + 1];
};

struct lws_lec_pctx;
typedef struct lws_lec_pctx lws_lec_pctx_t;

enum lws_lec_pctx_ret {
	LWS_LECPCTX_RET_FINISHED		= 0,
	LWS_LECPCTX_RET_AGAIN,
	LWS_LECPCTX_RET_FAIL
};

enum cbp_state {
	CBPS_IDLE,
	CBPS_PC1,
	CBPS_PC2,
	CBPS_PC3,
	CBPS_STRING_BODY,
	CBPS_NUM_LIT,
	CBPS_STRING_LIT,
	CBPS_CONTYPE,
};

typedef struct lws_lec_pctx {
	uint8_t			stack[16];
	uint8_t			vaa[16];
	uint8_t			indet[16];
	uint8_t			scratch[24];
	uint8_t			*start;
	uint8_t			*buf;
	uint8_t			*end;
	const uint8_t		*ongoing_src;
	uint64_t		ongoing_len;
	uint64_t		ongoing_done;
	struct lecp_item	item;
	size_t			used;
	int			opaque[4];
	enum cbp_state		state;
	unsigned int		fmt_pos;
	uint8_t			sp;
	uint8_t			scratch_len;
	uint8_t			escflag;
	uint8_t			_long;
	uint8_t			vaa_pos;
	uint8_t			dotstar;
} lws_lec_pctx_t;

LWS_VISIBLE LWS_EXTERN void
lws_lec_int(lws_lec_pctx_t *ctx, uint8_t opcode, uint8_t indet, uint64_t num);

LWS_VISIBLE LWS_EXTERN int
lws_lec_scratch(lws_lec_pctx_t *ctx);

LWS_VISIBLE LWS_EXTERN void
lws_lec_init(lws_lec_pctx_t *ctx, uint8_t *buf, size_t len);

LWS_VISIBLE LWS_EXTERN void
lws_lec_setbuf(lws_lec_pctx_t *ctx, uint8_t *buf, size_t len);

LWS_VISIBLE LWS_EXTERN enum lws_lec_pctx_ret
lws_lec_vsprintf (lws_lec_pctx_t *ctx, const char *format, va_list args);

LWS_VISIBLE LWS_EXTERN enum lws_lec_pctx_ret
lws_lec_printf(lws_lec_pctx_t *ctx, const char *format, ...);

LWS_VISIBLE LWS_EXTERN void
lecp_construct(struct lecp_ctx *ctx, lecp_callback cb, void *user,
	       const char * const *paths, unsigned char paths_count);

LWS_VISIBLE LWS_EXTERN void
lecp_destruct(struct lecp_ctx *ctx);

LWS_VISIBLE LWS_EXTERN int
lecp_parse(struct lecp_ctx *ctx, const uint8_t *cbor, size_t len);

LWS_VISIBLE LWS_EXTERN void
lecp_change_callback(struct lecp_ctx *ctx, lecp_callback cb);

LWS_VISIBLE LWS_EXTERN const char *
lecp_error_to_string(int e);

LWS_VISIBLE LWS_EXTERN void
lecp_parse_report_raw(struct lecp_ctx *ctx, int on);

LWS_VISIBLE LWS_EXTERN int
lecp_parse_map_is_key(struct lecp_ctx *ctx);

LWS_VISIBLE LWS_EXTERN int
lecp_parse_subtree(struct lecp_ctx *ctx, const uint8_t *in, size_t len);

LWS_VISIBLE LWS_EXTERN void
lws_singles2halfp(uint16_t *hp, uint32_t x);

LWS_VISIBLE LWS_EXTERN void
lws_halfp2singles(uint32_t *xp, uint16_t h);
// By GST @Date