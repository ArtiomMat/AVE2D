#include "a_ave.h"
#include <string.h>

int a_c;
char **a_v;

char *exepath;

void A_Init(int c, char **v)
{
	a_c = c;
	a_v = v;
	
	// exe length
	unsigned len = strlen(v[0]);
	for (len-=1; v[0][len-1] != '/' && v[0][len-1] != '\\'; len--);
	exepath = malloc(len);
	int i;
	for (i  = 0; i < len; i++)
		exepath[i] = v[0][i];
	exepath[i] = 0;
}

int A_FindArg(char *arg)
{
	for (int i = 0; i < a_c; i++)
	{
		if (!strcmp(arg, a_v[i]))
			return i;
	}
	return -1;
}

char relpath[256];
char *A_RelPath(const char *fp)
{
	strcpy(relpath, exepath);
	strcat(relpath, fp);
	return relpath;
}
