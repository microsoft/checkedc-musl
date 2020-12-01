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
	void *restrict a0 : itype(restrict _Array_ptr<void>))
{
	uint16_t ip _Checked[8];
	int i, j, v, d, brk=-1, need_v4=0;
	unsigned int s_cnt = 0;
	_Unchecked { s_cnt = strlen(s); }
	_Nt_array_ptr<const char> sw : bounds(s, s + s_cnt) = 0;
	_Unchecked {sw = _Assume_bounds_cast<_Nt_array_ptr<const char>>(s, count(s_cnt));}

	if (af==AF_INET) {
		_Array_ptr<unsigned char> a : bounds((_Array_ptr<unsigned char>)a0,
                                                           (_Array_ptr<unsigned char>)a0 + 4) = 0;
		_Unchecked { a = _Assume_bounds_cast<_Array_ptr<unsigned char>>(a0, count(4)); }
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

	_Array_ptr<unsigned char> a : bounds((_Array_ptr<unsigned char>)a0,
                                             (_Array_ptr<unsigned char>)a0 + 16) = 0;
	_Unchecked { a = _Assume_bounds_cast<_Array_ptr<unsigned char>>(a0, count(16)); }
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
		size_t arg_count = (size_t)(2*(i+1-brk));
		_Array_ptr<char> buf_dest : count(arg_count) =
			_Dynamic_bounds_cast<_Array_ptr<char>>(ip+brk+7-i, count(arg_count));
		_Array_ptr<char> buf_src : count(arg_count) =
			_Dynamic_bounds_cast<_Array_ptr<char>>(ip+brk, count(arg_count));
		memmove(buf_dest, buf_src, arg_count);
		for (j=0; j<7-i; j++) ip[brk+j] = 0;
	}
	for (j=0; j<8; j++) {
		*a++ = ip[j]>>8;
		*a++ = ip[j];
	}
	_Nt_array_ptr<char> arg_sw = _Dynamic_bounds_cast<_Nt_array_ptr<char>>(sw, count(0));
	if (need_v4 && inet_pton(AF_INET, arg_sw, a-4) <= 0) return 0;
	return 1;
}
