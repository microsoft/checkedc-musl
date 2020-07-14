#include <string.h>

size_t strnlen(const char *s : itype(_Nt_array_ptr<const char>) count(n), size_t n)
_Checked{
	_Nt_array_ptr<const char> p : count(n) = _Dynamic_bounds_cast<_Nt_array_ptr<const char>>(memchr(s, 0, n), count(n));
	return p ? p-s : n;
}
