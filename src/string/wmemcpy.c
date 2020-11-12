#include <wchar.h>

wchar_t *wmemcpy(wchar_t *restrict arg_d : itype(restrict _Nt_array_ptr<wchar_t>) count(arg_n),
                 const wchar_t *restrict arg_s : itype(restrict _Nt_array_ptr<const wchar_t>) count(arg_n),
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n)
_Checked{
	size_t n = arg_n;
	_Nt_array_ptr<wchar_t> d : bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                          (_Nt_array_ptr<wchar_t>)arg_d + arg_n)
                                 = arg_d;
	_Nt_array_ptr<const wchar_t> s : bounds((_Nt_array_ptr<const wchar_t>)arg_s,
                                                (_Nt_array_ptr<const wchar_t>)arg_s + arg_n)
                                       = arg_s;
	while (n--) *d++ = *s++;
	return arg_d;
}
