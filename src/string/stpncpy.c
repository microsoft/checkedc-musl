#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpncpy(char *restrict d : itype(restrict _Nt_array_ptr<char>) count(n),
                const char *restrict s : itype(restrict _Nt_array_ptr<const char>) count(n),
                size_t n)
  :itype(_Nt_array_ptr<char>) count(n)
_Checked{
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	_Nt_array_ptr<word> wd : count(n) = 0;
	_Nt_array_ptr<const word> ws : count(n) = 0;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (!n || !*s) goto tail;
		wd=(_Nt_array_ptr<word>)d; ws=(_Nt_array_ptr<const word>)s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d=(restrict _Nt_array_ptr<char>)wd; s=(restrict _Nt_array_ptr<const char>)ws;
	}
#endif
	for (; n && (*d=*s); n--, s++, d++);
tail:
	memset(d, 0, n);
	return d;
}

weak_alias(__stpncpy, stpncpy);

