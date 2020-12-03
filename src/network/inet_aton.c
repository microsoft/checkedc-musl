#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#pragma CHECKED_SCOPE on

int __inet_aton(const char *s0 : itype(_Nt_array_ptr<const char>),
	struct in_addr *dest : itype(_Ptr<struct in_addr>))
{
	unsigned int s_cnt = 0;
	_Unchecked { s_cnt = strlen(s0); }
	_Nt_array_ptr<const char> s : bounds(s0, s0 + s_cnt) = 0;
	_Unchecked {s = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s0, count(s_cnt));}
	// Struct in_addr has 32 bytes. Cast it to unsigned char pointer with count(4).
	_Array_ptr<unsigned char> d : count(4) = (_Array_ptr<unsigned char>)dest;
	unsigned long a _Checked[4] = { 0 };
	_Nt_array_ptr<char> z : bounds(s0, s0 + s_cnt) = 0;
	int i;

	for (i=0; i<4; i++) {
		_Unchecked {
			char *zp;
			a[i] = strtoul((const char *)s, &zp, 0);
			z = _Assume_bounds_cast<_Nt_array_ptr<char>>(zp, bounds(s0, s0 + s_cnt));
		}
		if (z==s || (*z && *z != '.') || !isdigit(*s))
			return 0;
		if (!*z) break;
		s=z+1;
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
