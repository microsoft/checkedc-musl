#define _GNU_SOURCE
#include <netdb.h>

struct servent *getservbyport(int port,
	const char *prots : itype(_Nt_array_ptr<const char>))
	: itype(_Ptr<struct servent>)
{
	static struct servent se;
	static long buf _Checked[32/sizeof(long)];
	_Ptr<struct servent> res = 0;
	if (getservbyport_r(port, prots, &se, (void *)buf, sizeof buf, &res))
		return 0;
	return &se;
}
