#include <resolv.h>

_Checked int dn_skipname(const unsigned char *s : bounds(s, end),
	const unsigned char *end : count(0))
{
	_Array_ptr<const unsigned char> p : bounds(p, end) = s;
	while (p < end)
		if (!*p) return p-s+1;
		else if (*p>=192)
			if (p+1<end) return p-s+2;
			else break;
		else
			if (end-p<*p+1) break;
			else p += *p + 1;
	return -1;
}
