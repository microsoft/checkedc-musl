#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#pragma CHECKED_SCOPE on

int __inet_aton(const char *s0 : itype(_Nt_array_ptr<const char>),
	struct in_addr *dest : itype(_Ptr<struct in_addr>))
{
	_Nt_array_ptr<const char> s = s0;
	_Array_ptr<unsigned char> d : count(4) = (_Array_ptr<unsigned char>)dest;
	unsigned long a _Checked[4] = { 0 };
	_Array_ptr<char> z; // Bounds are not known until the call to strtoul.
	int i;

	for (i=0; i<4; i++) {
		// TODO: strtoul does not accept checked pointers yet.
		// Since we cannot use address-of (&) operator on a checked pointer
		// with bounds, we must widen the bounds of z using _Assume_bounds_cast
		// after z is made point to the correct position in s through strtoul.
		_Nt_array_ptr<char> zp = 0;
		_Unchecked {
			a[i] = strtoul((char *)s, (char **)&z, 0);
			zp = _Assume_bounds_cast<_Nt_array_ptr<char>>(z, count(0));
		}
		if (z==s || (*zp && *zp != '.') || !isdigit(*s))
			return 0;
		if (!*zp) break;
		s=(_Nt_array_ptr<const char>)zp+1;
	}
	if (i==4) return 0;
	switch (i) {
	case 0:
		a[1] = a[0] & 0xffffff;
		a[0] >>= 24;
	case 1:
		a[2] = a[1] & 0xffff;
		a[1] >>= 16;
	case 2:
		a[3] = a[2] & 0xff;
		a[2] >>= 8;
	}
	for (i=0; i<4; i++) {
		if (a[i] > 255) return 0;
		d[i] = a[i];
	}
	return 1;
}

weak_alias(__inet_aton, inet_aton);
