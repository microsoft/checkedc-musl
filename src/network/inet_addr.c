#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#pragma CHECKED_SCOPE on

in_addr_t inet_addr(const char *p : itype(_Nt_array_ptr<const char>))
{
	struct in_addr a;
	if (!__inet_aton(p, &a)) return -1;
	return a.s_addr;
}
