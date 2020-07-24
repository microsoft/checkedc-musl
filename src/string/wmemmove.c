#include <wchar.h>
#include <stdint.h>

wchar_t *wmemmove(wchar_t *d : itype(_Nt_array_ptr<wchar_t>) count(n),
                  const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(n),
                  size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	_Nt_array_ptr<wchar_t> d0 : count(n) = d;
	if (d == s) return d;
	if ((uintptr_t)d-(uintptr_t)s < n * sizeof *d)
		while (n--) d[n] = s[n];
	else
		while (n--) *d++ = *s++;
	return d0;
}
