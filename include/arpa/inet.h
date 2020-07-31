#ifndef _ARPA_INET_H
#define	_ARPA_INET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <netinet/in.h>

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

in_addr_t inet_addr (const char *p : itype(_Nt_array_ptr<const char>));
in_addr_t inet_network (const char *p : itype(_Nt_array_ptr<const char>));
char *inet_ntoa (struct in_addr in) : count(15) itype(_Nt_array_ptr<char>);
int inet_pton (int af,
	const char *restrict s : itype(restrict _Nt_array_ptr<const char>),
	void *restrict a0 : byte_count(af==AF_INET ? 4 : 8));
const char *inet_ntop (int af,
	const void *restrict a0 : byte_count(16),
	char *restrict s : count(l),
	socklen_t l) : itype(_Nt_array_ptr<const char>);

int inet_aton (const char *s0 : itype(_Nt_array_ptr<const char>),
	struct in_addr *dest : itype(_Ptr<struct in_addr>));
struct in_addr inet_makeaddr(in_addr_t, in_addr_t);
in_addr_t inet_lnaof(struct in_addr);
in_addr_t inet_netof(struct in_addr);

#ifdef __cplusplus
}
#endif

#endif
