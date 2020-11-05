#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict arg_d : itype(restrict _Nt_array_ptr<wchar_t>)
                                           bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                                  (_Nt_array_ptr<wchar_t>)arg_d + arg_n),
                 const wchar_t *restrict s,
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n)
{

	size_t n = arg_n;
	_Nt_array_ptr<wchar_t> d : bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                          (_Nt_array_ptr<wchar_t>)arg_d + arg_n)
                                 = (_Nt_array_ptr<wchar_t>)arg_d;
	while (n && *s) n--, *d++ = *s++;
	wmemset(d, 0, n);
	return arg_d;
}
