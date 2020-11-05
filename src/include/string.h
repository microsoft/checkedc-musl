#ifndef STRING_H
#define STRING_H

#include "../../include/string.h"

hidden void *__memrchr(const void *, int, size_t);
hidden char *__stpcpy(char *, const char *);
hidden char *__stpncpy(char *restrict arg_d : itype(restrict _Array_ptr<char>)
                                       bounds((_Array_ptr<char>)arg_d, (_Array_ptr<char>)arg_d + arg_n),
                const char *restrict s,
                size_t arg_n)
  : itype(_Array_ptr<char>) count(arg_n);
hidden char *__strchrnul(const char *, int);

#endif
