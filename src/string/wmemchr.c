#include <wchar.h>

wchar_t *wmemchr(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(n),
                 wchar_t c,
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	for (; n && *s != c; n--, s++);
	return n ? (_Nt_array_ptr<wchar_t>)s : 0;
}
