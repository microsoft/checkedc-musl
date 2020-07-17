#include <string.h>

char *strncpy(char *restrict d : itype(restrict _Nt_array_ptr<char>) count(n),
              const char *restrict s : itype(restrict _Nt_array_ptr<const char>) count(n),
              size_t n)
  : itype(_Nt_array_ptr<char>) count(n)
_Checked{
	__stpncpy(d, s, n);
	return d;
}
