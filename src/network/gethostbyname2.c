#define _GNU_SOURCE

#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>

struct hostent *gethostbyname2(const char *name : itype(_Nt_array_ptr<const char>),
	int af) : itype(_Ptr<struct hostent>)
{
	static struct hostent *h;
	size_t size = 63;
	_Ptr<struct hostent> res = 0;
	int err;
	do {
		free(h);
		h = malloc(size+=size+1);
		if (!h) {
			h_errno = NO_RECOVERY;
			return 0;
		}
		err = gethostbyname2_r(name, af, h,
			(void *)(h+1), size-sizeof *h, &res, &h_errno);
	} while (err == ERANGE);
	return err ? 0 : h;
}
