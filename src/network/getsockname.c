#include <sys/socket.h>
#include "syscall.h"
#pragma CHECKED_SCOPE on

int getsockname(int fd,
	struct sockaddr *restrict addr : byte_count(*len),
	socklen_t *restrict len : itype(restrict _Ptr<socklen_t>))
{
	return socketcall(getsockname, fd, addr, len, 0, 0, 0);
}
