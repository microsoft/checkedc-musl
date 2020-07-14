#include <sys/socket.h>
#include "syscall.h"
#pragma CHECKED_SCOPE on

ssize_t sendto(int fd,
	const void *buf : byte_count(len),
	size_t len,
	int flags,
	const struct sockaddr *addr : byte_count(alen),
	socklen_t alen)
{
	return socketcall_cp(sendto, fd, buf, len, flags, addr, alen);
}
