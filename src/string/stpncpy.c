#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpncpy(char *restrict d : itype(restrict _Array_ptr<char>) count(n),
                      const char *restrict s,
                      size_t n)
  : itype(_Array_ptr<char>) count(n)
{
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	_Array_ptr<word> wd : count(n) = 0;
//	_Array_ptr<const word> ws : count(n) = 0;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (!n || !*s) goto tail;
		wd=(_Array_ptr<word>)d; ws=(const void *)s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d=(restrict _Array_ptr<void>)wd; s=(const void *)ws;
	}
#endif
	for (; n && (*d=*s); n--, s++, d++);
tail:
	memset(d, 0, n);
	return (_Array_ptr<char>)d;
}

weak_alias(__stpncpy, stpncpy);

