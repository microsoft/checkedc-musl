#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memccpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n),
               const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n),
               int c,
               size_t n)
  : itype(_Array_ptr<void>) byte_count(n)
_Checked
{
        _Array_ptr<unsigned char> d : count(n) = (_Array_ptr<unsigned char>)dest;
        _Array_ptr<const unsigned char> s : count(n) = (_Array_ptr<const unsigned char>)src;

	c = (unsigned char)c;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	_Array_ptr<word> wd : count(n) = 0;
	_Array_ptr<const word> ws : count(n) = 0;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s)!=c; n--, s++, d++);
		if ((uintptr_t)s & ALIGN) goto tail;
		size_t k = ONES * c;
		wd= (_Array_ptr<void>) d; ws= (_Array_ptr<const void>) s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws^k);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d= (_Array_ptr<void>) wd; s= (_Array_ptr<const void>) ws;
	}
#endif
	for (; n && (*d=*s)!=c; n--, s++, d++);
tail:
	if (n) return d+1;
	return 0;
}
