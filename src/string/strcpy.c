#include <string.h>

char *strcpy(char *restrict dest, const char *restrict src : itype(restrict _Nt_array_ptr<const char>))
{
	__stpcpy(dest, src);
	return dest;
}
