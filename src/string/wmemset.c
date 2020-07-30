#include <wchar.h>

wchar_t *wmemset(wchar_t *d : itype(_Nt_array_ptr<wchar_t>) count(n),
                 wchar_t c,
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	_Nt_array_ptr<wchar_t> ret : count(n) = d;
	while (n--) *d++ = c;
	return ret;
}
