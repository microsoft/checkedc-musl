#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict d : count(n), const wchar_t *restrict s, size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
{
	_Nt_array_ptr<wchar_t> a : count(n) = (_Nt_array_ptr<wchar_t>)d;
	size_t n1 = n;
	_Array_ptr<wchar_t> restrict d1 : bounds(d, d + n) = d;
	while (n1 && *s) n1--, *d1++ = *s++;
	wmemset((wchar_t *)d1, 0, n1);
	return a;
}
