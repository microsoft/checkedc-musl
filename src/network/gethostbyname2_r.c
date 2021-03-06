#define _GNU_SOURCE

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdint.h>
#include "lookup.h"

int gethostbyname2_r(const char *name : itype(_Nt_array_ptr<const char>),
	int af,
	struct hostent *h : itype(_Ptr<struct hostent>),
	char *arg_buf : count(buflen),
	size_t buflen,
	struct hostent **res : itype(_Ptr<_Ptr<struct hostent>>),
	int *err : itype(_Ptr<int>))
{
	struct address addrs _Checked[MAXADDRS];
	char canon _Nt_checked[256];
	int i, cnt;
	size_t align, need;

	*res = 0;
	cnt = __lookup_name(addrs, canon, name, af, AI_CANONNAME);
	if (cnt<0) switch (cnt) {
	case EAI_NONAME:
		*err = HOST_NOT_FOUND;
		return ENOENT;
	case EAI_AGAIN:
		*err = TRY_AGAIN;
		return EAGAIN;
	default:
	case EAI_FAIL:
		*err = NO_RECOVERY;
		return EBADMSG;
	case EAI_MEMORY:
	case EAI_SYSTEM:
		*err = NO_RECOVERY;
		return errno;
	}

	h->h_addrtype = af;
	h->h_length = af==AF_INET6 ? 16 : 4;

	_Array_ptr<char> buf : bounds(arg_buf, arg_buf + buflen) = arg_buf;
	/* Align buffer */
	align = -(uintptr_t)buf & sizeof(char *)-1;

	size_t name_len = strlen(name);
	size_t canon_len = strlen(canon);
	need = 4*sizeof(char *);
	need += (cnt + 1) * (sizeof(char *) + h->h_length);
	need += name_len+1;
	need += canon_len+1;
	need += align;

	if (need > buflen) return ERANGE;

	buf += align;
	h->h_aliases = (void *)buf;
	buf += 3*sizeof(char *);
	h->h_addr_list = (void *)buf;
	buf += (cnt+1)*sizeof(char *);

	for (i=0; i<cnt; i++) {
		h->h_addr_list[i] = (void *)buf;
		buf += h->h_length;
		memcpy(h->h_addr_list[i], addrs[i].addr, h->h_length);
	}
	h->h_addr_list[i] = 0;

	h->h_name = h->h_aliases[0] = buf;
	strncpy(h->h_name, (const char *)canon, canon_len+1);
	buf += strlen(h->h_name)+1;

	if (strcmp(h->h_name, name)) {
		h->h_aliases[1] = buf;
		strncpy(h->h_aliases[1], name, name_len+1);
		buf += strlen(h->h_aliases[1])+1;
	} else h->h_aliases[1] = 0;

	h->h_aliases[2] = 0;

	*res = h;
	return 0;
}
