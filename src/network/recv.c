#include <sys/socket.h>
#pragma CHECKED_SCOPE on

ssize_t recv(int fd,
	void *buf : byte_count(len),
	size_t len, int flags)
{
	return recvfrom(fd, buf, len, flags, 0, 0);
}
