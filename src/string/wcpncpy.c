#include <wchar.h>

wchar_t *wcpncpy(wchar_t *restrict d : itype(restrict _Nt_array_ptr<wchar_t>) count(n),
                 const wchar_t *restrict s : itype(restrict _Nt_array_ptr<const wchar_t>),
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n)
_Checked{
	return wcsncpy(d, s, n) + wcsnlen(s, n);
}
