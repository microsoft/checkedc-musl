#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#pragma CHECKED_SCOPE on

static int hexval(unsigned c)
{
	if (c-'0'<10) return c-'0';
	c |= 32;
	if (c-'a'<6) return c-'a'+10;
	return -1;
}

// This function converts the character string s into a network
// address structure in the af address family, then copies the network
// address structure to a0.  The af argument must be either AF_INET or
// AF_INET6.  a0 is written in network byte order.
int inet_pton(int af,
	const char *restrict s : itype(restrict _Nt_array_ptr<const char>),
	void *restrict a0 : byte_count(af==AF_INET ? 4 : 8))
{
	uint16_t ip _Checked[8];
	// Length of a that will be written.
	const int count = af==AF_INET ? 4 : 16;
	_Array_ptr<unsigned char> a : bounds(a0, (_Array_ptr<unsigned char>)a0 + count) = a0;
	int i, j, v, d, brk=-1, need_v4=0;

	// The length of s is unknown but it is 4 at max. Currently, the compiler
	// cannot automatically widen its bounds in the for loop. So here we assume
	// it has length 4 using _Assume_bounds_cast, which is the best we can do.
	_Nt_array_ptr<const char> sw : count(4) = 0;
	_Unchecked {
		sw = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, count(4));
	}
	if (af==AF_INET) {
		for (i=0; i<4; i++) {
			for (v=j=0; j<3 && sw[j] && isdigit(sw[j]); j++)
				v = 10*v + sw[j]-'0';
			if (j==0 || (j>1 && sw[0]=='0') || v>255) return 0;
			a[i] = v;
			if (sw[j]==0 && i==3) return 1;
			if (sw[j]!='.') return 0;
			sw += j+1;
		}
		return 0;
	} else if (af!=AF_INET6) {
		errno = EAFNOSUPPORT;
		return -1;
	}

	if (*sw==':' && *++sw!=':') return 0;

	for (i=0; ; i++) {
		if (sw[0]==':' && brk<0) {
			brk=i;
			ip[i&7]=0;
			if (!*++sw) break;
			if (i==7) return 0;
			continue;
		}
		for (v=j=0; j<4 && (d=hexval(sw[j]))>=0; j++)
			v=16*v+d;
		if (j==0) return 0;
		ip[i&7] = v;
		if (!sw[j] && (brk>=0 || i==7)) break;
		if (i==7) return 0;
		if (sw[j]!=':') {
			if (sw[j]!='.' || (i<6 && brk<0)) return 0;
			need_v4=1;
			i++;
			break;
		}
		sw += j+1;
	}
	if (brk>=0) {
		memmove(ip+brk+7-i, ip+brk, 2*(i+1-brk));
		for (j=0; j<7-i; j++) ip[brk+j] = 0;
	}
	for (j=0; j<8; j++) {
		*a++ = ip[j]>>8;
		*a++ = ip[j];
	}
	if (need_v4 && inet_pton(AF_INET, sw, a-4) <= 0) return 0;
	return 1;
}
