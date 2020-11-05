#include <string.h>
#include <stdint.h>

#ifdef __GNUC__
typedef __attribute__((__may_alias__)) size_t WT;
#define WS (sizeof(WT))
#endif

void *memmove (void *dest: itype(_Array_ptr<void>) byte_count(arg_n),
               const void *src : itype(_Array_ptr<const void>) byte_count(arg_n),
               size_t arg_n)
  : byte_count(arg_n)
_Checked{
	size_t n = arg_n;
	_Array_ptr<char> d : bounds((_Array_ptr<char>)dest, (_Array_ptr<char>)dest + arg_n)
                           = (_Array_ptr<char>)dest;
	_Array_ptr<const char> s : bounds((_Array_ptr<const char>)src, (_Array_ptr<const char>)src + arg_n)
                                 = (_Array_ptr<const char>)src;

	if (d==s) return d;
	if ((uintptr_t)s-(uintptr_t)d-n <= -2*n) return memcpy(d, s, n);

	if (d<s) {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			for (; n>=WS; n-=WS, d+=WS, s+=WS) *(_Array_ptr<WT>)d = *(_Array_ptr<WT>)s;
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
			while (n>=WS) n-=WS, *(_Array_ptr<WT>)(d+n) = *(_Array_ptr<WT>)(s+n);
		}
#endif
		while (n) n--, d[n] = s[n];
	}

	return dest;
}
