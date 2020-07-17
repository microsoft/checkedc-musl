#include <string.h>

int strncmp(const char *_l : itype(_Nt_array_ptr<const char>) count(n),
            const char *_r : itype(_Nt_array_ptr<const char>) count(n),
            size_t n)
_Checked{

	_Nt_array_ptr<const unsigned char> l : count(n) = (_Nt_array_ptr<const unsigned char>)_l;
	_Nt_array_ptr<const unsigned char> r : count(n) = (_Nt_array_ptr<const unsigned char>)_r;
	if (!n--) return 0;
	for (; *l && *r && n && *l == *r ; l++, r++, n--);
	return *l - *r;
}
