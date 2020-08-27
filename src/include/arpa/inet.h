#ifndef ARPA_INET_H
#define ARPA_INET_H

#include "../../../include/arpa/inet.h"

hidden int __inet_aton(const char *s0 : itype(_Nt_array_ptr<const char>),
	struct in_addr *dest : itype(_Ptr<struct in_addr>));

#endif
