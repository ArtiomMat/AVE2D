#include "a_ave.h"
#include "cl_main.h"

int main(int c, char **v)
{
	A_Init(c, v);
	CL_Init();
	CL_Close(0);
	return 0;
}
