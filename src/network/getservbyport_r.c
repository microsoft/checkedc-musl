#define _GNU_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int getservbyport_r(int port,
	const char *prots : itype(_Nt_array_ptr<const char>),
	struct servent *se : itype(_Ptr<struct servent>),
	char *arg_buf : count(arg_buflen),
	size_t arg_buflen,
	struct servent **res : itype(_Ptr<_Ptr<struct servent>>))
{
	int i;
	size_t buflen = arg_buflen;
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_port = port,
	};

	_Array_ptr<char> buf : bounds(arg_buf, arg_buf + arg_buflen) = arg_buf;
	if (!prots) {
		int r = getservbyport_r(port, "tcp", se, buf, buflen, res);
		if (r) r = getservbyport_r(port, "udp", se, buf, buflen, res);
		return r;
	}
	*res = 0;

	/* Align buffer */
	i = (uintptr_t)buf & sizeof(char *)-1;
	if (!i) i = sizeof(char *);
	if (buflen < 3*sizeof(char *)-i)
		return ERANGE;
	buf += sizeof(char *)-i;
	buflen -= sizeof(char *)-i;

	if (strcmp(prots, "tcp") && strcmp(prots, "udp")) return EINVAL;

	se->s_port = port;
	se->s_proto = (char *)prots;
	se->s_aliases = (void *)buf;
	buf += 2*sizeof(char *);
	buflen -= 2*sizeof(char *);
	se->s_aliases[1] = 0;
	se->s_aliases[0] = se->s_name =(char *)buf;

	switch (getnameinfo((void *)&sin, sizeof sin, 0, 0, (char *)buf, buflen,
		strcmp(prots, "udp") ? 0 : NI_DGRAM)) {
	case EAI_MEMORY:
	case EAI_SYSTEM:
		return ENOMEM;
	default:
		return ENOENT;
	case 0:
		break;
	}

	/* A numeric port string is not a service record. */
	if (strtol((char *)buf, 0, 10)==ntohs(port)) return ENOENT;

	*res = se;
	return 0;
}
