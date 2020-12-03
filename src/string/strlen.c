#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strlen(const char *arg_s : itype(_Nt_array_ptr<const char>))
_Checked{
	_Nt_array_ptr<const char> s1 : bounds(arg_s, s1) = arg_s;
#ifdef __GNUC__
	_Unchecked
	{
		const char *s = (const char *)s1;
		typedef size_t __attribute__((__may_alias__)) word;
		const word *w;
		for (; (uintptr_t)s % ALIGN; s++) if (!*s) return s-arg_s;
		for (w = (const void *)s; !HASZERO(*w); w++);
		s = (const void *)w;
		s1 = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, bounds(arg_s, s));
	}
#endif

	for (; *s1; s1++);
	return s1-arg_s;
}
