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
// Is not compiled by checkedc compiler
	_Unchecked{size_t *wd;}

	if (!n--) goto finish;
// This part is GCC Specific code and uses unchecked pointer,
// Clang compiler should not compile this part.
#ifdef __GNUC__
#ifndef __clang__
	typedef size_t __attribute__((__may_alias__)) word;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
		if (n && *s) {
			wd=(void *)d; ws=(const void *)s;
			for (; n>=sizeof(size_t) && !HASZERO(*ws);
			       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
			d=(void *)wd; s=(const void *)ws;
		}
	}
#endif
#endif
	for (; n && (*d=*s); n--, s++, d++);
	*d = 0;
finish:
// The unchecked scope will be removed when the compiler has support
// to convert strlen function
	_Unchecked{return d-d0 + strlen(s);}
}
