#include <string.h>
#include <stdint.h>
#include <limits.h>

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memchr (const void *src : itype(_Array_ptr<const void>) byte_count(n),
              int c,
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n)
_Checked
{
	_Array_ptr<const unsigned char> s : count(n) = (_Array_ptr<const unsigned char>) src;
	c = (unsigned char)c;

// This part is GCC Specific code and uses unchecked pointer,
// Clang compiler should not compile this part.
#ifdef __GNUC__
#ifndef __clang__
	for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
	if (n && *s != c) {
		typedef size_t __attribute__((__may_alias__)) word;
		const word *w;
		size_t k = ONES * c;
		for (w = (const void *)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
		s = (const void *)w;
	}
#endif
#endif
	for (; n && *s != c; s++, n--);
	return n ? (_Array_ptr<void>) s : 0;
}
