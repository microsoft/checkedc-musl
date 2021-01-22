#include <string.h>
#include <stdint.h>
#include <limits.h>

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strnlen_array_ptr(const char *s : itype(_Array_ptr<const char>) count(n), size_t n)
_Checked{
	_Array_ptr<const char> p : count(n) = memchr(s, 0, n);
	return p ? p-s : n;
}


size_t strnlen(const char *arg_s : itype(_Nt_array_ptr<const char>), size_t arg_n)
_Checked{
	size_t n = arg_n;
        _Nt_array_ptr<const char> s1 : bounds(arg_s, s1) = arg_s;
#ifdef __GNUC__
        _Unchecked {
        const char *s = (const char *)s1;
        typedef size_t __attribute__((__may_alias__)) word;
        const word *w;
        for (; n && (uintptr_t)s & ALIGN; s++, n--) if (!*s) return s-arg_s;
        for (w = (const void *)s; n >=SS && !HASZERO(*w); w++, n-=SS);
        s = (const void *)w;
        s1 = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, bounds(arg_s, s));
        }
#endif

        for (; n && *s1; s1++, n--);
        return n ? s1-arg_s : arg_n;
}
