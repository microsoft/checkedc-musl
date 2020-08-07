#include <wchar.h>

size_t wcsnlen(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>), size_t n)
_Checked{
	_Nt_array_ptr<const wchar_t> z : count(n) = wmemchr(s, 0, n);
	size_t n1 = n;
	if (z) n1 = z-s;
	return n1;
}
