#define _BSD_SOURCE
#include <string.h>
#include <strings.h>

void bcopy(const void *s1 : itype(_Array_ptr<const void>) byte_count(n),
           const void *s2 : itype(_Array_ptr<const void>) byte_count(n),
           size_t n)
_Checked{
	memmove(s2, s1, n);
}
