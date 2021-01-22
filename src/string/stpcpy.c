#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpcpy(char *restrict d, const char *restrict arg_s : itype(restrict _Nt_array_ptr<const char>))
{
	_Nt_array_ptr<const char> s1 : bounds(arg_s, s1) = arg_s;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	word *wd;
	const word *ws;
	if ((uintptr_t)s1 % ALIGN == (uintptr_t)d % ALIGN) {
		const char *s = (const char *)s1;
		for (; (uintptr_t)s % ALIGN; s++, d++)
			if (!(*d=*s)) return d;
		wd=(void *)d; ws=(const void *)s;
		for (; !HASZERO(*ws); *wd++ = *ws++);
		d=(void *)wd; s=(const void *)ws;
		s1 = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, bounds(arg_s, s));
	}
#endif

	// TODO: The casting to const char * of s1 below is necessary until the issue with the
	// widening of bounds of s1 (#938) is fixed in the compiler, in order to avoid test failures.
	for (; (*d=*(const char *)s1); s1++, d++);

	return d;
}

weak_alias(__stpcpy, stpcpy);
