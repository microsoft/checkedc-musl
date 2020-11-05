#include <wchar.h>

wchar_t *wmemset(wchar_t *arg_d : itype(_Nt_array_ptr<wchar_t>) count(arg_n),
                 wchar_t c,
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n)
_Checked{
	size_t n = arg_n;
	_Nt_array_ptr<wchar_t> d : bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                          (_Nt_array_ptr<wchar_t>)arg_d + arg_n)
                                 = arg_d;
	while (n--) *d++ = c;
	return arg_d;
}
