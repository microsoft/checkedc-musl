#include <string.h>

char *strcpy(char *restrict dest : itype(restrict _Nt_array_ptr<char>),
             const char *restrict src : itype(restrict _Nt_array_ptr<const char>))
      : itype(_Nt_array_ptr<char>)
_Checked
{
	__stpcpy(dest, src);
	return dest;
}
