#include <unistd.h>

void swab(const void *restrict _src : itype(restrict _Array_ptr<const void>) byte_count(n),
          void *restrict _dest : itype(restrict _Array_ptr<void>) byte_count(n),
          ssize_t n)
_Checked{
	_Array_ptr<const char> src : count(n / sizeof(char)) = (_Array_ptr<const char>) _src;
	_Array_ptr<char> dest: count(n /sizeof(char)) = (_Array_ptr<char>) _dest;
	for (; n>1; n-=2) {
		dest[0] = src[1];
		dest[1] = src[0];
		dest += 2;
		src += 2;
	}
}
