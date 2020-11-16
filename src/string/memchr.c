#include <string.h>
#include <stdint.h>
#include <limits.h>

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memchr (const void *src : itype(_Array_ptr<const void>) byte_count(arg_n),
              int c,
              size_t arg_n)
  : itype(_Array_ptr<void>) byte_count(arg_n)

_Checked
{
	size_t n = arg_n;
	_Array_ptr<const unsigned char> s : bounds((_Array_ptr<const unsigned char>)src,
                                                  (_Array_ptr<const unsigned char>)src + arg_n)
                                          = (_Array_ptr<const unsigned char>) src;
	c = (unsigned char)c;

#ifdef __GNUC__
	for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
	if (n && *s != c) {
		typedef size_t __attribute__((__may_alias__)) word;
		_Array_ptr<const word> w : bounds((_Array_ptr<const unsigned char>)src,
                                           (_Array_ptr<const unsigned char>)src + arg_n) rel_align(unsigned char)
                                         = 0;
		size_t k = ONES * c;
		for (w = (_Array_ptr<const void>)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
		s = (_Array_ptr<const void>)w;
	}
#endif
	for (; n && *s != c; s++, n--);
	return n ? (_Array_ptr<void>) s : 0;
}
