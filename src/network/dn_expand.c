#include <resolv.h>

_Checked int __dn_expand(const unsigned char *base : bounds(base, end),
	const unsigned char *end : itype(_Array_ptr<const unsigned char>),
	const unsigned char *src : bounds(base, end),
	char *arg_dest : count(space),
	int space)
{
	_Array_ptr<const unsigned char> p : bounds(base, end) = src;
	int len = -1, i, j;
	if (p==end || space <= 0) return -1;
	const _Ptr<const char> dbegin = _Dynamic_bounds_cast<_Ptr<const char>>(arg_dest);
	const _Array_ptr<const char> dend : bounds(unknown) = arg_dest + (space > 254 ? 254 : space);
	_Array_ptr<char> dest : bounds(arg_dest, arg_dest + space) = arg_dest;
	/* detect reference loop using an iteration counter */
	for (i=0; i < end-base; i+=2) {
		/* loop invariants: p<end, dest<=dend */
		if (*p & 0xc0) {
			if (p+1==end) return -1;
			j = ((p[0] & 0x3f) << 8) | p[1];
			if (len < 0) len = p+2-src;
			if (j >= end-base) return -1;
			p = base+j;
		} else if (*p) {
			if (dest != dbegin) *dest++ = '.';
			j = *p++;
			if (j >= end-p || j >= dend-dest) return -1;
			while (j--) *dest++ = *p++;
		} else {
			*dest = 0;
			if (len < 0) len = p+1-src;
			return len;
		}
	}
	return -1;
}

weak_alias(__dn_expand, dn_expand);
