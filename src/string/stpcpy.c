#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpcpy(char *restrict arg_d : itype(restrict _Nt_array_ptr<char>),
             const char *restrict arg_s : itype(restrict _Nt_array_ptr<const char>))
      : itype(_Nt_array_ptr<char>)
_Checked
{
	_Nt_array_ptr<const char> s1 : bounds(arg_s, s1) = arg_s;
	_Nt_array_ptr<char> d1 : bounds(arg_d, d1) = arg_d;
#ifdef __GNUC__
	_Unchecked
	{
		typedef size_t __attribute__((__may_alias__)) word;
		word *wd;
		const word *ws;
		if ((uintptr_t)s1 % ALIGN == (uintptr_t)d1 % ALIGN) {
			const char *s = (const char *)s1;
			char *d = (char *)d1;
			for (; (uintptr_t)s % ALIGN; s++, d++)
				if (!(*d=*s)) return d;
			wd=(void *)d; ws=(const void *)s;
			for (; !HASZERO(*ws); *wd++ = *ws++);
			d=(void *)wd; s=(const void *)ws;
			s1 = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, bounds(arg_s, s));
			d1 = _Assume_bounds_cast<_Nt_array_ptr<char>>(d, bounds(arg_d, d));
		}
	}
#endif

	// The unchecked block and the casting to char * of d1 and s1 below is necessary until the issue with
	// the widening of bounds of d1 and s1 is fixed in the compiler, in order to avoid test failures.
	_Unchecked { for (; (*(char *)d1=*(const char *)s1); s1++, d1++); }

	return d1;
}

weak_alias(__stpcpy, stpcpy);
