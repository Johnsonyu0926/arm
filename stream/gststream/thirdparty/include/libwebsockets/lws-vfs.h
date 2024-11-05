/ Score: 97

#ifndef __LWS_FILE_OPS_H__
#define __LWS_FILE_OPS_H__

#define LWS_FOP_OPEN open
#define LWS_FOP_CLOSE close
#define LWS_FOP_SEEK_CUR seek_cur
#define LWS_FOP_READ read
#define LWS_FOP_WRITE write

#define LWS_FOP_FLAGS_MASK ((1 << 23) - 1)
#define LWS_FOP_FLAG_COMPR_ACCEPTABLE_GZIP (1 << 24)
#define LWS_FOP_FLAG_COMPR_IS_GZIP (1 << 25)
#define LWS_FOP_FLAG_MOD_TIME_VALID (1 << 26)
#define LWS_FOP_FLAG_VIRTUAL (1 << 27)

struct lws_plat_file_ops;

struct lws_fop_fd {
    lws_filefd_type fd;
    const struct lws_plat_file_ops *fops;
    void *filesystem_priv;
    lws_filepos_t pos;
    lws_filepos_t len;
    lws_fop_flags_t flags;
    uint32_t mod_time;
};
typedef struct lws_fop_fd *lws_fop_fd_t;

struct lws_fops_index {
    const char *sig;
    uint8_t len;
};

struct lws_plat_file_ops {
    lws_fop_fd_t (*LWS_FOP_OPEN)(const struct lws_plat_file_ops *fops, const char *filename, const char *vpath, lws_fop_flags_t *flags);
    int (*LWS_FOP_CLOSE)(lws_fop_fd_t *fop_fd);
    lws_fileofs_t (*LWS_FOP_SEEK_CUR)(lws_fop_fd_t fop_fd, lws_fileofs_t offset_from_cur_pos);
    int (*LWS_FOP_READ)(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len);
    int (*LWS_FOP_WRITE)(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len);

    struct lws_fops_index fi[3];

    const struct lws_plat_file_ops *next;
};

LWS_VISIBLE LWS_EXTERN struct lws_plat_file_ops * LWS_WARN_UNUSED_RESULT lws_get_fops(struct lws_context *context);

LWS_VISIBLE LWS_EXTERN void lws_set_fops(struct lws_context *context, const struct lws_plat_file_ops *fops);

LWS_VISIBLE LWS_EXTERN lws_filepos_t LWS_WARN_UNUSED_RESULT lws_vfs_tell(lws_fop_fd_t fop_fd);

LWS_VISIBLE LWS_EXTERN lws_filepos_t LWS_WARN_UNUSED_RESULT lws_vfs_get_length(lws_fop_fd_t fop_fd);

LWS_VISIBLE LWS_EXTERN uint32_t LWS_WARN_UNUSED_RESULT lws_vfs_get_mod_time(lws_fop_fd_t fop_fd);

LWS_VISIBLE LWS_EXTERN lws_fileofs_t lws_vfs_file_seek_set(lws_fop_fd_t fop_fd, lws_fileofs_t offset);

LWS_VISIBLE LWS_EXTERN lws_fileofs_t lws_vfs_file_seek_end(lws_fop_fd_t fop_fd, lws_fileofs_t offset);

extern struct lws_plat_file_ops fops_zip;

LWS_VISIBLE LWS_EXTERN lws_fop_fd_t LWS_WARN_UNUSED_RESULT lws_vfs_file_open(const struct lws_plat_file_ops *fops, const char *vfs_path, lws_fop_flags_t *flags);

static LWS_INLINE int lws_vfs_file_close(lws_fop_fd_t *fop_fd) {
    if (*fop_fd && (*fop_fd)->fops) return (*fop_fd)->fops->LWS_FOP_CLOSE(fop_fd);
    return 0;
}

static LWS_INLINE lws_fileofs_t lws_vfs_file_seek_cur(lws_fop_fd_t fop_fd, lws_fileofs_t offset) {
    return fop_fd->fops->LWS_FOP_SEEK_CUR(fop_fd, offset);
}

static LWS_INLINE int LWS_WARN_UNUSED_RESULT lws_vfs_file_read(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len) {
    return fop_fd->fops->LWS_FOP_READ(fop_fd, amount, buf, len);
}

static LWS_INLINE int LWS_WARN_UNUSED_RESULT lws_vfs_file_write(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len) {
    return fop_fd->fops->LWS_FOP_WRITE(fop_fd, amount, buf, len);
}

LWS_VISIBLE LWS_EXTERN lws_fop_fd_t _lws_plat_file_open(const struct lws_plat_file_ops *fops, const char *filename, const char *vpath, lws_fop_flags_t *flags);

LWS_VISIBLE LWS_EXTERN int _lws_plat_file_close(lws_fop_fd_t *fop_fd);

LWS_VISIBLE LWS_EXTERN lws_fileofs_t _lws_plat_file_seek_cur(lws_fop_fd_t fop_fd, lws_fileofs_t offset);

LWS_VISIBLE LWS_EXTERN int _lws_plat_file_read(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len);

LWS_VISIBLE LWS_EXTERN int _lws_plat_file_write(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len);

LWS_VISIBLE LWS_EXTERN int lws_alloc_vfs_file(struct lws_context *context, const char *filename, uint8_t **buf, lws_filepos_t *amount);

#endif // __LWS_FILE_OPS_H__

// By GST @Date