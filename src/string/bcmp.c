#define _BSD_SOURCE
#include <string.h>
#include <strings.h>

int bcmp(const void *s1 : itype(_Array_ptr<const void>) byte_count(n),
         const void *s2 : itype(_Array_ptr<const void>) byte_count(n),
         size_t n)
_Checked{
	return memcmp(s1, s2, n);
}
