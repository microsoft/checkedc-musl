#include <string.h>

void *__memrchr(const void *m : itype(_Array_ptr<const void>) byte_count(n), int c, size_t n) : itype(_Array_ptr<void>) byte_count(n)
{
	_Array_ptr<const unsigned char> s : count(n) = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(m, count(n));
	c = (unsigned char)c;
	while (n--) if (s[n]==c) return (void *)(s+n);
	return 0;
}

weak_alias(__memrchr, memrchr);
