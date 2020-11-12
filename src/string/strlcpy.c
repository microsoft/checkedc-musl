#define _BSD_SOURCE
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strlcpy(char *arg_d : itype(_Nt_array_ptr<char>) count(arg_n),
               const char *s,
               size_t arg_n)
{
	// Renamed input arg d to arg_d, input arg n to arg_n in order to use them in bounds
	// declarations that remain valid through modifications to d and n in the loop body.
	size_t n = arg_n;
	_Nt_array_ptr<char> d : bounds(arg_d, arg_d + arg_n) = arg_d;
	_Nt_array_ptr<size_t> wd : bounds(arg_d, arg_d + arg_n) rel_align(char) = 0;
	if (!n--) goto finish;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
        const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (n && *s) {
			wd=(_Nt_array_ptr<size_t>)d; ws=(const void *)s;
			for (; n>=sizeof(size_t) && !HASZERO(*ws);
			       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
			d=(_Nt_array_ptr<char>)wd; s=(const void *)ws;
		}
	}
#endif
	for (; n && (*d=*s); n--, s++, d++);
	*d = 0;
finish:
	return d-arg_d + strlen(s);
}
