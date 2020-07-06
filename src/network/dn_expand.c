#include <resolv.h>

_Checked int __dn_expand(const unsigned char *base : bounds(base, end) itype(_Array_ptr<const unsigned char>),
	const unsigned char *end : itype(_Ptr<const unsigned char>),
	const unsigned char *src : bounds(src, end) itype(_Array_ptr<const unsigned char>),
	char *dest : count(space > 254 ? 254 : space) itype(_Array_ptr<char>),
	int space)
{
	// space0 = MIN(space, 254).
	int space0 = space > 254 ? 254 : space;
	_Array_ptr<const unsigned char> p : bounds(src, end) = src;
	int len = -1, i, j;
	if (p==end || space <= 0) return -1;
	const _Array_ptr<const char> dbegin : count(space0) = dest;
	const _Array_ptr<const char> dend : count(0) = dest + space0;
	// dst loops through bounds(dbegin, dend).
	_Array_ptr<char> dst : bounds(dbegin, dend) = dest;
	/* detect reference loop using an iteration counter */
	for (i=0; i < end-base; i+=2) {
		/* loop invariants: p<end, dst<dend */
		if (*p & 0xc0) {
			if (p+1==end) return -1;
			j = ((p[0] & 0x3f) << 8) | p[1];
			if (len < 0) len = p+2-src;
			if (j >= end-base) return -1;
			p = base+j;
		} else if (*p) {
			if (dst != dbegin) *dst++ = '.';
			j = *p++;
			if (j >= end-p || j >= dend-dst) return -1;
			while (j--) *dst++ = *p++;
		} else {
			*dst = 0;
			if (len < 0) len = p+1-src;
			return len;
		}
	}
	return -1;
}

weak_alias(__dn_expand, dn_expand);
