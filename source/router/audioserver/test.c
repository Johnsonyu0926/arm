#include <stdio.h>
int main()
{
	char s[]="08:1";
	int i,j;
	sscanf(s,"%d:%d", &i, &j);
	printf("%d %d\n", i,j);
	return 0;
}
