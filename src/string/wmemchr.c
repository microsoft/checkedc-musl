#include <wchar.h>

wchar_t *wmemchr(const wchar_t *arg_s : itype(_Nt_array_ptr<const wchar_t>) count(arg_n),
                 wchar_t c,
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n)
_Checked{
	size_t n = arg_n;
	_Nt_array_ptr<const wchar_t> s : bounds((_Nt_array_ptr<const wchar_t>)arg_s,
                                                (_Nt_array_ptr<const wchar_t>)arg_s + arg_n)
                                       = arg_s;
	for (; n && *s != c; n--, s++);
	return n ? (_Nt_array_ptr<wchar_t>)s : 0;
}
