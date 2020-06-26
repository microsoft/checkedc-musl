#include <string.h>

int memcmp (const void *vl : itype(_Array_ptr<const void>) byte_count(n), const void *vr : itype(_Array_ptr<const void>) byte_count(n), size_t n)
{
	_Array_ptr<const unsigned char> l : count(n) = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(vl, count(n));
	_Array_ptr<const unsigned char> r : count(n) = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(vr, count(n));
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}
