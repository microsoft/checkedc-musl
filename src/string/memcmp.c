#include <string.h>

int memcmp (const void *vl : itype(_Array_ptr<const void>) byte_count(arg_n),
            const void *vr : itype(_Array_ptr<const void>) byte_count(arg_n),
            size_t arg_n)
_Checked
{
	size_t n = arg_n;
	_Array_ptr<const unsigned char> l : bounds((_Array_ptr<const unsigned char>)vl,
                                                   (_Array_ptr<const unsigned char>)vl + arg_n)
                                          = (_Array_ptr<const unsigned char>)vl;
	_Array_ptr<const unsigned char> r : bounds((_Array_ptr<const unsigned char>)vr, 
                                                   (_Array_ptr<const unsigned char>)vr + arg_n)
                                          = (_Array_ptr<const unsigned char>)vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}
