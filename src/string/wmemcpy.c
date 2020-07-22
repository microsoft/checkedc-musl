#include <wchar.h>

wchar_t *wmemcpy(wchar_t *restrict d : itype(restrict _Nt_array_ptr<wchar_t>) count(n),
                 const wchar_t *restrict s : itype(restrict _Nt_array_ptr<const wchar_t>),
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	_Nt_array_ptr<wchar_t> a : count(n) = d;
	while (n--) *d++ = *s++;
	return a;
}
