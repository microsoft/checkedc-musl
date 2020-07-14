#ifndef STRING_H
#define STRING_H

#include "../../include/string.h"

hidden void *__memrchr(const void *, int, size_t);
hidden char *__stpcpy(char *, const char *);
hidden char *__stpncpy(char *__restrict : itype(__restrict _Nt_array_ptr<char>) count(n),
                       const char *__restrict : itype(__restrict _Nt_array_ptr<const char>) count(n),
                       size_t n)
  : itype(_Nt_array_ptr<char>) count(n);
hidden char *__strchrnul(const char *, int);

#endif
