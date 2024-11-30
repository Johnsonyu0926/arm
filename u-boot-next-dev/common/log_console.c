//log_console.c

#include <common.h>
#include <log.h>

static int log_console_emit(struct log_device *ldev, struct log_rec *rec)
{
	puts(rec->msg);

	return 0;
}

LOG_DRIVER(console) = {
	.name	= "console",
	.emit	= log_console_emit,
};
