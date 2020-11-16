#include <wchar.h>

size_t wcsnlen(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(arg_n), size_t arg_n)
_Checked{
	size_t n = arg_n;
	_Nt_array_ptr<const wchar_t> z : count(arg_n) = wmemchr(s, 0, n);
	if (z) n = z-s;
	return n;
}
