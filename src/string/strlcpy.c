#define _BSD_SOURCE
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strlcpy(char *d : itype(_Nt_array_ptr<char>) count(n),
               const char *s : itype(_Nt_array_ptr<const char>) count(n),
               size_t n)
_Checked{
       _Nt_array_ptr<char> d0 : count(n) = d;
       _Nt_array_ptr<size_t> wd : count(n) = 0;
	if (!n--) goto finish;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
       _Nt_array_ptr<const word> ws : count(n) = 0;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (n && *s) {
			//cast from (void *) to _Nt_array_ptr<size_t>
			//cast from (const void *) to _Nt_array_ptr<const void>
			wd=(_Nt_array_ptr<size_t>)d; ws=(_Nt_array_ptr<const word>)s;
			for (; n>=sizeof(size_t) && !HASZERO(*ws);
			       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
			d=(_Nt_array_ptr<char>)wd; s=(_Nt_array_ptr<const char>)ws;
		}
	}
#endif
	for (; n && (*d=*s); n--, s++, d++);
	*d = 0;
finish:
	return d-d0 + strlen(s);
}
