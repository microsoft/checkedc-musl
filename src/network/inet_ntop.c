#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#pragma CHECKED_SCOPE on

// This function converts the network address structure a0 in the af
// address family into a character string.  The resulting string is
// copied to the buffer pointed to by s, which must be a non-null
// pointer.  The caller specifies the number of bytes available in this
// buffer in the argument l.  a0 must have at least 16 bytes.
const char *inet_ntop(int af,
	const void *restrict a0 : byte_count(af==AF_INET ? 4 : 16),
	char *restrict s : itype(restrict _Nt_array_ptr<char>) count(l),
	socklen_t l) : itype(_Nt_array_ptr<const char>)
{
	int i, j, max, best;
	char buf _Nt_checked[100];

	switch (af) {
	case AF_INET: {
		_Array_ptr<const unsigned char> a : count(4) = 0;
		a = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(a0, count(4));
		_Unchecked {
			if (snprintf(s, l, "%d.%d.%d.%d", a[0],a[1],a[2],a[3]) < l)
				return s;
		}
		break;
	}
	case AF_INET6: {
		_Array_ptr<const unsigned char> a : count(16) = 0;
		a = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(a0, count(16));
		if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\377\377", 12)) _Unchecked {
			snprintf((char *)buf, sizeof buf,
				"%x:%x:%x:%x:%x:%x:%x:%x",
				256*a[0]+a[1],256*a[2]+a[3],
				256*a[4]+a[5],256*a[6]+a[7],
				256*a[8]+a[9],256*a[10]+a[11],
				256*a[12]+a[13],256*a[14]+a[15]);
		}
		else _Unchecked {
			snprintf((char *)buf, sizeof buf,
				"%x:%x:%x:%x:%x:%x:%d.%d.%d.%d",
				256*a[0]+a[1],256*a[2]+a[3],
				256*a[4]+a[5],256*a[6]+a[7],
				256*a[8]+a[9],256*a[10]+a[11],
				a[12],a[13],a[14],a[15]);
		}
		/* Replace longest /(^0|:)[:0]{2,}/ with "::" */
		for (i=best=0, max=2; buf[i]; i++) {
			if (i && buf[i] != ':') continue;
			_Unchecked {
				j = strspn((char *)buf+i, ":0");
			}
			if (j>max) best=i, max=j;
		}
		if (max>3) {
			buf[best] = buf[best+1] = ':';
			size_t arg_count = (size_t)(i-best-max+1);
			_Array_ptr<char> buf_dest : count(arg_count) =
                                _Dynamic_bounds_cast<_Array_ptr<char>>(buf+best+2, count(arg_count));
			_Array_ptr<char> buf_src : count(arg_count) =
                                _Dynamic_bounds_cast<_Array_ptr<char>>(buf+best+max, count(arg_count));
			memmove(buf_dest, buf_src, arg_count);
		}
		size_t buf_len = strlen(buf);
		if (buf_len < l) {
			_Unchecked {strncpy(s, (const char *)buf, buf_len+1);}
			return s;
		}
		break;
	}
	default:
		errno = EAFNOSUPPORT;
		return 0;
	}
	errno = ENOSPC;
	return 0;
}
