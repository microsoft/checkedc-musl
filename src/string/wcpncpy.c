#include <wchar.h>

wchar_t *wcpncpy(wchar_t *restrict d : itype(restrict _Array_ptr<wchar_t>)
                                       bounds((_Array_ptr<wchar_t>)d, (_Array_ptr<wchar_t>)d + n),
                 const wchar_t *restrict s,
                 size_t n)
  : itype(_Array_ptr<wchar_t>) count(n)
{
	return wcsncpy(d, s, n) + wcsnlen(s, n);
}
