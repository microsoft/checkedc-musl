#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpncpy(char *restrict arg_d : itype(restrict _Array_ptr<char>)
                                       bounds((_Array_ptr<char>)arg_d, (_Array_ptr<char>)arg_d + arg_n),
                const char *restrict s,
                size_t arg_n)
  : itype(_Array_ptr<char>) count(arg_n)
{
	size_t n = arg_n;
	_Array_ptr<char> d : bounds((_Array_ptr<char>)arg_d, (_Array_ptr<char>)arg_d + arg_n)
                           = arg_d;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	_Array_ptr<word> wd : bounds((_Array_ptr<char>)arg_d, (_Array_ptr<char>)arg_d + arg_n) rel_align(char)
                            = 0;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (!n || !*s) goto tail;
		wd=(_Array_ptr<void>)d; ws=(const void *)s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d=(_Array_ptr<void>)wd; s=(const void *)ws;
	}
#endif
	for (; n && (*d=*s); n--, s++, d++);
tail:
	memset(d, 0, n);
	return d;
}

weak_alias(__stpncpy, stpncpy);

