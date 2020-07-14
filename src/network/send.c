#include <sys/socket.h>
#pragma CHECKED_SCOPE on

ssize_t send(int fd,
	const void *buf : byte_count(len),
	size_t len, int flags)
{
	return sendto(fd, buf, len, flags, 0, 0);
}
