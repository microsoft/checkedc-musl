#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict d : itype(restrict _Nt_array_ptr<wchar_t>) count(n), const wchar_t *restrict s, size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
{

// n and d are used in the declared bounds of d. But both of them are modified in the loop and hence the declared bounds of d no
// longer remain valid. So we need to use a temp variable in this function.
	size_t temp_n = n;
	_Nt_array_ptr<wchar_t> a : count(n) = (_Nt_array_ptr<wchar_t>)d;
	_Nt_array_ptr<wchar_t> temp_d : count(n) = (_Nt_array_ptr<wchar_t>)d;
	while (temp_n && *s) temp_n--, *temp_d++ = *s++;
	wmemset(temp_d, 0, temp_n);
	return a;
}
