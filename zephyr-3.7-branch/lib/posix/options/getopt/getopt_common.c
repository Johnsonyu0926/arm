//zephyr-3.7-branch/lib/posix/options/getopt/getopt_common.c
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#if CONFIG_SHELL_GETOPT
#include <zephyr/sys/iterable_sections.h>
#endif

#include "getopt.h"

/* Referring below variables is not thread safe. They reflect getopt state
 * only when 1 thread is using getopt.
 * When more threads are using getopt please call getopt_state_get to know
 * getopt state for the current thread.
 */
int opterr = 1;  /* if error message should be printed */
int optind = 1;  /* index into parent argv vector */
int optopt;      /* character checked for validity */
int optreset;    /* reset getopt */
char *optarg;    /* argument associated with option */

/* Common state for all threads that did not have own getopt state. */
static struct getopt_state m_getopt_common_state = {
	.opterr = 1,
	.optind = 1,
	.optopt = 0,
	.optreset = 0,
	.optarg = NULL,

	.place = "", /* EMSG */

#if CONFIG_GETOPT_LONG
	.nonopt_start = -1, /* first non option argument (for permute) */
	.nonopt_end = -1,   /* first option after non options (for permute) */
#endif
};

/**
 * @brief Update global getopt state
 *
 * This function is not thread safe. All threads using getopt are calling
 * this function.
 *
 * @param state Pointer to the getopt state
 */
void z_getopt_global_state_update(struct getopt_state *state)
{
	opterr = state->opterr;
	optind = state->optind;
	optopt = state->optopt;
	optreset = state->optreset;
	optarg = state->optarg;
}

/**
 * @brief Get the getopt state for the current thread
 *
 * It is internal getopt API function, it shall not be called by the user.
 *
 * @return Pointer to the getopt state
 */
struct getopt_state *getopt_state_get(void)
{
#if CONFIG_SHELL_GETOPT
	k_tid_t tid;

	tid = k_current_get();
	STRUCT_SECTION_FOREACH(shell, sh) {
		if (tid == sh->ctx->tid) {
			return &sh->ctx->getopt;
		}
	}
#endif
	/* If not a shell thread return a common pointer */
	return &m_getopt_common_state;
}

//GST