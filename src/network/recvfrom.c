#include <sys/socket.h>
#include "syscall.h"
#pragma CHECKED_SCOPE on

ssize_t recvfrom(int fd,
	void *restrict buf : byte_count(len),
	size_t len,
	int flags,
	struct sockaddr *restrict addr : byte_count(*alen),
	socklen_t *restrict alen : itype(restrict _Ptr<socklen_t>))
{
	return socketcall_cp(recvfrom, fd, buf, len, flags, addr, alen);
}
