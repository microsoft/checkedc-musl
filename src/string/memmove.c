#include <string.h>
#include <stdint.h>

#ifdef __GNUC__
typedef __attribute__((__may_alias__)) size_t WT;
#define WS (sizeof(WT))
#endif

void *memmove (void *dest: itype(_Array_ptr<void>) byte_count(n),
               const void *src : itype(_Array_ptr<const void>) byte_count(n),
               size_t n)
  : byte_count(n)
_Checked{
	_Array_ptr<char> d : count(n) = (_Array_ptr<char>)dest;
	_Array_ptr<const char> s : count(n) = (_Array_ptr<const char>)src;

	if (d==s) return d;
	if ((uintptr_t)s-(uintptr_t)d-n <= -2*n) return memcpy(d, s, n);

	if (d<s) {
// This part is GCC Specific code and uses unchecked pointer,
// Clang compiler should not compile this part.
#ifdef __GNUC__
#ifndef __clang__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			for (; n>=WS; n-=WS, d+=WS, s+=WS) *(WT *)d = *(WT *)s;
		}
#endif
#endif
		for (; n; n--) *d++ = *s++;
	} else {
// This part is GCC Specific code and uses unchecked pointer,
// Clang compiler should not compile this part.
#ifdef __GNUC__
#ifndef __clang__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)(d+n) % WS) {
				if (!n--) return dest;
				d[n] = s[n];
			}
			while (n>=WS) n-=WS, *(WT *)(d+n) = *(WT *)(s+n);
		}
#endif
#endif
		while (n) n--, d[n] = s[n];
	}

	return dest;
}
