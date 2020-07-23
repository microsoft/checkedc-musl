#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict d : itype(restrict _Nt_array_ptr<wchar_t>) count(n), const wchar_t *restrict s, size_t n)
{
	_Array_ptr<wchar_t> a : count(n) = (_Array_ptr<wchar_t>)d;
	while (n && *s) n--, *d++ = *s++;
	wmemset(d, 0, n);
	return d;
}
