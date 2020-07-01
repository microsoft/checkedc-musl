#ifndef RESOLV_H
#define RESOLV_H

#include "../../include/resolv.h"

hidden int __dn_expand(const unsigned char *base : bounds(base, end) itype(_Nt_array_ptr<const unsigned char>),
    const unsigned char *end : itype(_Ptr<const unsigned char>),
    const unsigned char *src : bounds(src, end) itype(_Nt_array_ptr<const unsigned char>),
    char *dest : count(space > 254 ? 254 : space) itype(_Nt_array_ptr<char>),
    int space);

hidden int __res_mkquery(int, const char *, int, int, const unsigned char *, int, const unsigned char*, unsigned char *, int);
hidden int __res_send(const unsigned char *, int, unsigned char *, int);
hidden int __res_msend(int, const unsigned char *const *, const int *, unsigned char *const *, int *, int);

#endif
