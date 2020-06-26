#include <string.h>
#include <stdint.h>

#ifdef __GNUC__
typedef __attribute__((__may_alias__)) size_t WT;
#define WS (sizeof(WT))
#endif

void *memmove (void * dest: itype(_Array_ptr<void>) byte_count(n), const void * : itype(_Array_ptr<const void>) , size_t n) : bounds(dest, (_Array_ptr<char>)dest + n)
{
	_Nt_array_ptr<char> d = _Dynamic_bounds_cast<_Nt_array_ptr<char>>(dest, bounds(dest, (_Array_ptr<char>)dest + n ));
	_Nt_array_ptr<const char> s = _Dynamic_bounds_cast<_Nt_array_ptr<const char>>(src, count(n));

	if (d==s) return d;
	if ((uintptr_t)s-(uintptr_t)d-n <= -2*n) return memcpy(d, s, n);

	if (d<s) {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			for (; n>=WS; n-=WS, d+=WS, s+=WS) *(WT *)d = *(WT *)s;
		}
#endif
		for (; n; n--) *d++ = *s++;
	} else {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)(d+n) % WS) {
				if (!n--) return dest;
				d[n] = s[n];
			}
			while (n>=WS) n-=WS, *(WT *)(d+n) = *(WT *)(s+n);
		}
#endif
		while (n) n--, d[n] = s[n];
	}

	return dest;
}
