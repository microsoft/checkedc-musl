#define _BSD_SOURCE
#include <string.h>

size_t strlcat(char *d : itype(_Nt_array_ptr<char>) count(n),
               const char *s : itype(_Nt_array_ptr<const char>) count(n),
               size_t n)
_Checked{
	size_t l = strnlen(d, n);
// The unchecked scope will be removed when the compiler has support
// to convert strlen function
	_Unchecked{if (l == n) return l + strlen(s);}
	return l + strlcpy(d+l, s, n-l);
}
