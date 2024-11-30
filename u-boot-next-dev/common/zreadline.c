//zreadline.c 
#include "zglobal.h"

#include <stdlib.h>
#include <stdio.h>
//#include <signal.h>
//#include <ctype.h>
#include <errno.h>

//#include "error.h"


/* Ward Christensen / CP/M parameters - Don't change these! */
#define TIMEOUT (-2)

static size_t readline_readnum;
static int readline_fd;
static char *readline_buffer;
int readline_left=0;
char *readline_ptr;
extern int read_data(int tout_in_100ms, char *buf, int size);

RETSIGTYPE
zreadline_alarm_handler(int dummy LRZSZ_ATTRIB_UNUSED)
{
	/* doesn't need to do anything */
}

/*
 * This version of readline is reasonably well suited for
 * reading many characters.
 *
 * timeout is in tenths of seconds
 */
int 
readline_internal(unsigned int timeout)
{
	unsigned int n=0;

	if (!no_timeout)
	{
		n = timeout/10;
		if (n < 2 && timeout!=1)
			n = 3;
		else if (n==0)
			n=1;
		if (Verbose > 5)
			vstringf("Calling read: alarm=%d  Readnum=%d ",
			  n, (u32)readline_readnum);
		//signal(SIGALRM, zreadline_alarm_handler); 
		//alarm(n);
	}
	else if (Verbose > 5)
		vstringf("Calling read: Readnum=%d ",
		  (u32)readline_readnum);

	readline_ptr=readline_buffer;
	readline_left=read_data(n*10, readline_ptr, readline_readnum);
	if (!no_timeout)
		;//alarm(0);
	if (readline_left>0 && bytes_per_error) {
		static long ct=0;
		static int mod=1;
		ct+=readline_left;
		while (ct>bytes_per_error) {
			readline_ptr[ct % bytes_per_error]^=mod;
			ct-=bytes_per_error;
			mod++;
			if (mod==256)
				mod=1;
		}
	}
	if (Verbose > 5) {
		vstringf("Read returned %d bytes\n", readline_left);
		//if (readline_left==-1)
		//	vstringf("errno=%d:%s\n", errno,strerror(errno));
		if (Verbose > 9 && readline_left>0) {
			int i,j;
			j=readline_left > 48 ? 48 : readline_left;
			vstring("    ");
			for (i=0;i<j;i++) {
				if (i%24==0 && i)
					vstring("\n    ");
				vstringf("%02x ", readline_ptr[i] & 0377);
			}
			vstringf("\n");
		}
	}
	if (readline_left < 1)
		return TIMEOUT;
	--readline_left;
	return (*readline_ptr++ & 0377);
}



void
readline_setup(int fd, size_t readnum, size_t bufsize)
{
	readline_fd=fd;
	readline_readnum=readnum;
	readline_buffer=malloc(bufsize > readnum ? bufsize : readnum);
	//if (!readline_buffer)
	//	error(1,0,_("out of memory"));
}

void
readline_purge(void)
{
	readline_left=0;
	return;
}

