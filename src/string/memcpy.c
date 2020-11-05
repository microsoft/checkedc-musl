#include <string.h>
#include <stdint.h>
#include <endian.h>

void *memcpy (void *__restrict dest : itype(__restrict _Array_ptr<void>)
                                      bounds((_Array_ptr<unsigned char>)dest,
                                             (_Array_ptr<unsigned char>)dest + arg_n),
              const void *__restrict src : itype(__restrict _Array_ptr<const void>)
                                           bounds((_Array_ptr<const unsigned char>)src,
                                                  (_Array_ptr<const unsigned char>)src + arg_n),
              size_t arg_n)
  : itype(_Array_ptr<void>) byte_count(arg_n)
_Checked
{
	size_t n = arg_n;
        _Array_ptr<unsigned char> d : bounds((_Array_ptr<unsigned char>)dest,
                                             (_Array_ptr<unsigned char>)dest + arg_n)
                                    = dest;
        _Array_ptr<const unsigned char> s : bounds((_Array_ptr<const unsigned char>)src,
                                                   (_Array_ptr<const unsigned char>)src + arg_n)
                                          = src;

#ifdef __GNUC__
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define LS >>
#define RS <<
#else
#define LS <<
#define RS >>
#endif
	typedef uint32_t __attribute__((__may_alias__)) u32;
	uint32_t w, x;

	for (; (uintptr_t)s % 4 && n; n--) *d++ = *s++;

	if ((uintptr_t)d % 4 == 0) {
		for (; n>=16; s+=16, d+=16, n-=16) {
			*(_Array_ptr<u32>)(d+0) = *(_Array_ptr<u32>)(s+0);
			*(_Array_ptr<u32>)(d+4) = *(_Array_ptr<u32>)(s+4);
			*(_Array_ptr<u32>)(d+8) = *(_Array_ptr<u32>)(s+8);
			*(_Array_ptr<u32>)(d+12) = *(_Array_ptr<u32>)(s+12);
		}
		if (n&8) {
			*(_Array_ptr<u32>)(d+0) = *(_Array_ptr<u32>)(s+0);
			*(_Array_ptr<u32>)(d+4) = *(_Array_ptr<u32>)(s+4);
			d += 8; s += 8;
		}
		if (n&4) {
			*(_Array_ptr<u32>)(d+0) = *(_Array_ptr<u32>)(s+0);
			d += 4; s += 4;
		}
		if (n&2) {
			*d++ = *s++; *d++ = *s++;
		}
		if (n&1) {
			*d = *s;
		}
		return dest;
	}

	if (n >= 32) switch ((uintptr_t)d % 4) {
	case 1:
		w = *(_Array_ptr<u32>)s;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		n -= 3;
		for (; n>=17; s+=16, d+=16, n-=16) {
			x = *(_Array_ptr<u32>)(s+1);
			*(_Array_ptr<u32>)(d+0) = (w LS 24) | (x RS 8);
			w = *(_Array_ptr<u32>)(s+5);
			*(_Array_ptr<u32>)(d+4) = (x LS 24) | (w RS 8);
			x = *(_Array_ptr<u32>)(s+9);
			*(_Array_ptr<u32>)(d+8) = (w LS 24) | (x RS 8);
			w = *(_Array_ptr<u32>)(s+13);
			*(_Array_ptr<u32>)(d+12) = (x LS 24) | (w RS 8);
		}
		break;
	case 2:
		w = *(_Array_ptr<u32>)s;
		*d++ = *s++;
		*d++ = *s++;
		n -= 2;
		for (; n>=18; s+=16, d+=16, n-=16) {
			x = *(_Array_ptr<u32>)(s+2);
			*(_Array_ptr<u32>)(d+0) = (w LS 16) | (x RS 16);
			w = *(_Array_ptr<u32>)(s+6);
			*(_Array_ptr<u32>)(d+4) = (x LS 16) | (w RS 16);
			x = *(_Array_ptr<u32>)(s+10);
			*(_Array_ptr<u32>)(d+8) = (w LS 16) | (x RS 16);
			w = *(_Array_ptr<u32>)(s+14);
			*(_Array_ptr<u32>)(d+12) = (x LS 16) | (w RS 16);
		}
		break;
	case 3:
		w = *(_Array_ptr<u32>)s;
		*d++ = *s++;
		n -= 1;
		for (; n>=19; s+=16, d+=16, n-=16) {
			x = *(_Array_ptr<u32>)(s+3);
			*(_Array_ptr<u32>)(d+0) = (w LS 8) | (x RS 24);
			w = *(_Array_ptr<u32>)(s+7);
			*(_Array_ptr<u32>)(d+4) = (x LS 8) | (w RS 24);
			x = *(_Array_ptr<u32>)(s+11);
			*(_Array_ptr<u32>)(d+8) = (w LS 8) | (x RS 24);
			w = *(_Array_ptr<u32>)(s+15);
			*(_Array_ptr<u32>)(d+12) = (x LS 8) | (w RS 24);
		}
		break;
	}
	if (n&16) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&8) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&4) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&2) {
		*d++ = *s++; *d++ = *s++;
	}
	if (n&1) {
		*d = *s;
	}
	return dest;
#endif

	for (; n; n--) *d++ = *s++;
	return dest;
}
