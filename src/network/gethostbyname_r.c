#define _GNU_SOURCE

#include <sys/socket.h>
#include <netdb.h>

int gethostbyname_r(const char *name : itype(_Nt_array_ptr<const char>),
	struct hostent *h : itype(_Ptr<struct hostent>),
	char *buf : count(buflen),
	size_t buflen,
	struct hostent **res : itype(_Ptr<_Ptr<struct hostent>>),
	int *err : itype(_Ptr<int>))
{
	return gethostbyname2_r(name, AF_INET, h, buf, buflen, res, err);
}
