#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict d : itype(restrict _Nt_array_ptr<wchar_t>) count(n),
                 const wchar_t *restrict s : itype(restrict _Nt_array_ptr<const wchar_t>),
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	_Nt_array_ptr<wchar_t> a : bounds(d, d+n) = d;
	while (n && *s) n--, *d++ = *s++;
	wmemset(d, 0, n);
	return a;
}
