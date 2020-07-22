#include <wchar.h>

int wmemcmp(const wchar_t *l : itype(_Nt_array_ptr<const wchar_t>) count(n),
            const wchar_t *r : itype(_Nt_array_ptr<const wchar_t>) count(n),
            size_t n)
_Checked{
	for (; n && *l==*r; n--, l++, r++);
	return n ? *l-*r : 0;
}
