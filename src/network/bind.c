#include <sys/socket.h>
#include "syscall.h"
#pragma CHECKED_SCOPE on

int bind(int fd,
	const struct sockaddr *addr : byte_count(len),
	socklen_t len)
{
	return socketcall(bind, fd, addr, len, 0, 0, 0);
}
