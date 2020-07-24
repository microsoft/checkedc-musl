#define _BSD_SOURCE
#include <string.h>

void explicit_bzero(void *d : itype(_Array_ptr<void>) byte_count(n), size_t n)
_Checked{
	d = memset(d, 0, n);
	__asm__ __volatile__ ("" : : "r"(d) : "memory");
}
