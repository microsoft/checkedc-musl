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
	const void *restrict a0 : byte_count(16),
	char *restrict s : count(l),
	socklen_t l) : itype(_Nt_array_ptr<const char>)
{
	_Array_ptr<const unsigned char> a : count(16) = a0;
	int i, j, max, best;
	char buf _Nt_checked[100];

	switch (af) {
	case AF_INET:
		_Unchecked {
			if (snprintf(s, l, "%d.%d.%d.%d", a[0],a[1],a[2],a[3]) < l)
				return s;
		}
		break;
	case AF_INET6:
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
			memmove(buf+best+2, buf+best+max, i-best-max+1);
		}
		_Unchecked {
			if (strlen((char *)buf) < l) {
				strcpy((char *)s, (char *)buf);
				return s;
			}
		}
		break;
	default:
		errno = EAFNOSUPPORT;
		return 0;
	}
	errno = ENOSPC;
	return 0;
}
