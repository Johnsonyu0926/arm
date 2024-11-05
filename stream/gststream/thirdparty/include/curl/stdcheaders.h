#ifndef CURLINC_STDCHEADERS_H
#define CURLINC_STDCHEADERS_H
/***************************************************************************/

#include <sys/types.h>
#include <stdio.h>  // Added to ensure FILE is defined
#include <string.h> // Added to ensure size_t is defined

size_t fread(void *, size_t, size_t, FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);

int strcasecmp(const char *, const char *);
int strncasecmp(const char *, const char *, size_t);

#endif /* CURLINC_STDCHEADERS_H */

// By GST @Date