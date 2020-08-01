#define _GNU_SOURCE

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <inttypes.h>

int gethostbyaddr_r(const void *a : byte_count(l),
	socklen_t l,
	int af,
	struct hostent *h : itype(_Ptr<struct hostent>),
	char *buf_ori : count(buflen),
	size_t buflen,
	struct hostent **res : itype(_Ptr<_Ptr<struct hostent>>),
	int *err : itype(_Ptr<int>))
{
	union {
		struct sockaddr_in sin;
		struct sockaddr_in6 sin6;
	} sa = { .sin.sin_family = af };
	socklen_t sl = af==AF_INET6 ? sizeof sa.sin6 : sizeof sa.sin;
	int i;

	*res = 0;

	/* Load address argument into sockaddr structure */
	if (af==AF_INET6 && l==16) memcpy(&sa.sin6.sin6_addr, a, 16);
	else if (af==AF_INET && l==4) memcpy(&sa.sin.sin_addr, a, 4);
	else {
		*err = NO_RECOVERY;
		return EINVAL;
	}

	/* Align buffer and check for space for pointers and ip address */
	_Array_ptr<char> buf : bounds(buf_ori, buf_ori + buflen) = buf_ori;
	i = (uintptr_t)buf & sizeof(char *)-1;
	if (!i) i = sizeof(char *);
	if (buflen <= 5*sizeof(char *)-i + l) return ERANGE;
	buf += sizeof(char *)-i;
	buflen -= 5*sizeof(char *)-i + l;

	h->h_addr_list = (void *)buf;
	buf += 2*sizeof(char *);
	h->h_aliases = (void *)buf;
	buf += 2*sizeof(char *);

	h->h_addr_list[0] = buf;
	memcpy(h->h_addr_list[0], a, l);
	buf += l;
	h->h_addr_list[1] = 0;
	h->h_aliases[0] = buf;
	h->h_aliases[1] = 0;

	switch (getnameinfo((void *)&sa, sl, (char *)buf, buflen, 0, 0, 0)) {
	case EAI_AGAIN:
		*err = TRY_AGAIN;
		return EAGAIN;
	case EAI_OVERFLOW:
		return ERANGE;
	default:
	case EAI_MEMORY:
	case EAI_SYSTEM:
	case EAI_FAIL:
		*err = NO_RECOVERY;
		return errno;
	case 0:
		break;
	}

	h->h_addrtype = af;
	h->h_length = l;
	h->h_name = h->h_aliases[0];
	*res = h;
	return 0;
}
