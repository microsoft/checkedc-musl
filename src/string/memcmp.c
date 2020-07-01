#include <string.h>

int memcmp (const void *vl : itype(_Array_ptr<const void>) byte_count(n),
	    const void *vr : itype(_Array_ptr<const void>) byte_count(n), 
	    size_t n)
_Checked
{
	_Array_ptr<const unsigned char> l : count(n) = (_Array_ptr<const unsigned char>)vl;
	_Array_ptr<const unsigned char> r : count(n) = (_Array_ptr<const unsigned char>)vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}
