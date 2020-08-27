#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__stpncpy(char *restrict d : itype(restrict _Array_ptr<char>) count(n),
                const char *restrict s,
                size_t n)
  : itype(_Array_ptr<char>) count(n)
{
// n and d are used in the declared bounds of d. But both are modified in the
// loop and hence the declared bounds of d no
// longer remain valid. So we need to use a temp variable in this function.
	size_t temp_n = n;
	restrict _Array_ptr<char> temp_d : count(n) = d;
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	_Array_ptr<word> wd : count(n) = 0;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && temp_n && (*temp_d=*s); temp_n--, s++, temp_d++);
		if (!temp_n || !*s) goto tail;
		wd=(_Array_ptr<word>)temp_d; ws=(const void *)s;
		for (; temp_n>=sizeof(size_t) && !HASZERO(*ws);
		       temp_n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		temp_d=(restrict _Array_ptr<void>)wd; s=(const void *)ws;
	}
#endif
	for (; temp_n && (*temp_d=*s); temp_n--, s++, temp_d++);
tail:
	memset(temp_d, 0, temp_n);
	return (_Array_ptr<char>)temp_d;
}

weak_alias(__stpncpy, stpncpy);

