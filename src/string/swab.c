#include <unistd.h>

void swab(const void *restrict _src : itype(restrict _Array_ptr<const void>)
                                      bounds((_Array_ptr<const char>)_src, (_Array_ptr<const char>)_src + arg_n),
          void *restrict _dest : itype(restrict _Array_ptr<void>)
                                 bounds((_Array_ptr<char>)_dest, (_Array_ptr<char>)_dest + arg_n),
          ssize_t arg_n)
_Checked{
	// Renamed input arg n to arg_n in order to use it in bounds declarations
	// that remain valid through modifications to n in the loop body.
	ssize_t n = arg_n;
	_Array_ptr<const char> src : bounds((_Array_ptr<const char>)_src, (_Array_ptr<const char>)_src + arg_n)
                                   = (_Array_ptr<const char>)_src;
	_Array_ptr<char> dest: bounds((_Array_ptr<char>)_dest, (_Array_ptr<char>)_dest + arg_n)
                             = (_Array_ptr<char>)_dest;

	for (; n>1; n-=2) {
		dest[0] = src[1];
		dest[1] = src[0];
		dest += 2;
		src += 2;
	}
}
