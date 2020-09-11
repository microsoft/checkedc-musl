#include <wchar.h>

size_t wcsnlen(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>), size_t n)
_Checked{

	// n is used in the declared bounds of d. But n is modified in the loop and
	// hence the declared bounds of d no
	// longer remain valid. So we need to use a temp variable in this function.
	size_t temp_n = n;
	_Nt_array_ptr<const wchar_t> z : count(n) = wmemchr(s, 0, n);
	if (z) temp_n = z-s;
	return temp_n;
}
