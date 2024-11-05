enum lws_enum_stdinouterr {
    LWS_STDIN = 0,
    LWS_STDOUT = 1,
    LWS_STDERR = 2,
};

enum lws_cgi_hdr_state {
    LCHS_HEADER,
    LCHS_CR1,
    LCHS_LF1,
    LCHS_CR2,
    LCHS_LF2,
    LHCS_RESPONSE,
    LHCS_DUMP_HEADERS,
    LHCS_PAYLOAD,
    LCHS_SINGLE_0A,
};

struct lws_cgi_args {
    struct lws **stdwsi;
    enum lws_enum_stdinouterr ch;
    unsigned char *data;
    enum lws_cgi_hdr_state hdr_state;
    int len;
};

#ifdef LWS_WITH_CGI
LWS_VISIBLE LWS_EXTERN int
lws_cgi(struct lws *wsi, const char * const *exec_array,
    int script_uri_path_len, int timeout_secs,
    const struct lws_protocol_vhost_options *mp_cgienv);

LWS_VISIBLE LWS_EXTERN int
lws_cgi_write_split_stdout_headers(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN int
lws_cgi_kill(struct lws *wsi);

LWS_VISIBLE LWS_EXTERN struct lws *
lws_cgi_get_stdwsi(struct lws *wsi, enum lws_enum_stdinouterr ch);

#endif
// By GST @Date