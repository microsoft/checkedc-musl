#include <wchar.h>
#include <stdint.h>

wchar_t *wmemmove(wchar_t *arg_d : itype(_Nt_array_ptr<wchar_t>) count(arg_n),
                  const wchar_t *arg_s : itype(_Nt_array_ptr<const wchar_t>) count(arg_n),
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
	if (d == s) return d;
	if ((uintptr_t)d-(uintptr_t)s < n * sizeof *d)
		while (n--) d[n] = s[n];
	else
		while (n--) *d++ = *s++;
	return arg_d;
}
