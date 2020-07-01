#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memccpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n), const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n), int c,  size_t n) : itype(_Array_ptr<void>) byte_count(n)
{
        _Array_ptr<unsigned char> d : count(n) = (_Array_ptr<unsigned char>)dest;
        _Array_ptr<const unsigned char> s : count(n) = (_Array_ptr<const unsigned char>)src;

	c = (unsigned char)c;

//make sure the code is not compiling by clang	
#ifdef __GNUC_ && !defined __clang__
	typedef size_t __attribute__((__may_alias__)) word;
	word *wd;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s)!=c; n--, s++, d++);
		if ((uintptr_t)s & ALIGN) goto tail;
		size_t k = ONES * c;
		wd=(void *)d; ws=(const void *)s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws^k);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d=(void *)wd; s=(const void *)ws;
	}
#endif
	for (; n && (*d=*s)!=c; n--, s++, d++);
tail:
	if (n) return d+1;
	return 0;
}
