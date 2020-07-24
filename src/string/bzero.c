#define _BSD_SOURCE
#include <string.h>
#include <strings.h>

void bzero(void *s :itype(_Array_ptr<void>) byte_count(n), size_t n)
_Checked{
	memset(s, 0, n);
}
