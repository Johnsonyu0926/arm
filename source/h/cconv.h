#ifndef __CCONV_H__
#define __CCONV_H__

#include <cstdio>
#include "doorsbase.h"

#define BUFLEN 2000 

class CConv {
public:
    BOOL Utf8Encoding(const char* szSrc, char* szDest);

private:
    BOOL outbin(const char* outbin, char* szDest);
};

#endif // __CCONV_H__