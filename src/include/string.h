#ifndef STRING_H
#define STRING_H

#include "../../include/string.h"

hidden void *__memrchr(const void *, int, size_t);
hidden char *__stpcpy(char *restrict : itype(restrict _Nt_array_ptr<char>),
                      const char *restrict : itype(restrict _Nt_array_ptr<const char>))
      : itype(_Nt_array_ptr<char>);
hidden char *__stpncpy(char *restrict d : itype(restrict _Array_ptr<char>) count(n),
                       const char *restrict s,
                       size_t n)
  :itype(_Array_ptr<char>) count(n);
hidden char *__strchrnul(const char *, int);

#endif
