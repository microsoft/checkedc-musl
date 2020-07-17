#include <strings.h>
#include <ctype.h>

int strncasecmp(const char *_l : itype(_Nt_array_ptr<const char>) count(n),
                const char *_r : itype(_Nt_array_ptr<const char>) count(n),
                size_t n)
_Checked{
	//Change cast of (void *)  to cast of (_Nt_array_ptr<const unsigned char>)
	_Nt_array_ptr<const unsigned char> l : count(n)= (_Nt_array_ptr<const unsigned char>)_l;
	_Nt_array_ptr<const unsigned char> r : count(n)= (_Nt_array_ptr<const unsigned char>)_r;
	if (!n--) return 0;
	for (; *l && *r && n && (*l == *r || tolower(*l) == tolower(*r)); l++, r++, n--);
	return tolower(*l) - tolower(*r);
}

int __strncasecmp_l(const char *l : itype(_Nt_array_ptr<const char>) count(n),
                    const char *r : itype(_Nt_array_ptr<const char>) count(n),
                    size_t n,
                    locale_t loc)
_Checked{
	return strncasecmp(l, r, n);
}

weak_alias(__strncasecmp_l, strncasecmp_l);
