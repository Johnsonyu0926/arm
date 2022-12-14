#include <stdio.h>
#include "doorsbase.h"

#define BUFLEN 2000 
class CConv
{
	public:
		BOOL Utf8Encoding(char* szSrc,char* szDest);

	private:
		BOOL outbin(char* outbin,char* szDest);
};
