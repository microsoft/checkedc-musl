#define _GNU_SOURCE
#include <netdb.h>

struct servent *getservbyname(const char *name : itype(_Nt_array_ptr<const char>),
	const char *prots : itype(_Nt_array_ptr<const char>))
	: itype(_Ptr<struct servent>)
{
	static struct servent se;
	static _Array_ptr<char> buf _Checked[2];
	_Ptr<struct servent> res = 0;
	if (getservbyname_r(name, prots, &se, (_Array_ptr<char>)buf, sizeof buf, &res))
		return 0;
	return &se;
}
