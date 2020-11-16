#include <string.h>

void *__memrchr(const void *m : itype(_Array_ptr<const void>) byte_count(arg_n),
                int c,
                size_t arg_n)
  : itype(_Array_ptr<void>) byte_count(arg_n)
_Checked
{
	size_t n = arg_n;
	_Array_ptr<const unsigned char> s : count(arg_n) = (_Array_ptr<const unsigned char>)m;
	c = (unsigned char)c;
	while (n--) if (s[n]==c) return (_Array_ptr<void>)(s+n);
	return 0;
}

weak_alias(__memrchr, memrchr);
