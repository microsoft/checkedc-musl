#ifndef RESOLV_H
#define RESOLV_H

#include "../../include/resolv.h"

hidden int __dn_expand(const unsigned char *base : bounds(base, end) itype(_Array_ptr<const unsigned char>),
    const unsigned char *end : itype(_Ptr<const unsigned char>),
    const unsigned char *src : bounds(src, end) itype(_Array_ptr<const unsigned char>),
    char *dest : count(space > 254 ? 254 : space) itype(_Array_ptr<char>),
    int space);

hidden int __res_mkqueryint(int op,
	const char *dname : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	const unsigned char *data : count(datalen),
	int datalen,
	const unsigned char *newrr,
	unsigned char *buf : count(buflen),
	int buflen);
hidden int __res_send(const unsigned char *msg : count(msglen),
	int msglen,
	unsigned char *answer : count(anslen),
	int anslen);
hidden int __res_msend(int nqueries,
    const unsigned char *const *queries : itype(_Ptr<_Array_ptr<const unsigned char> const>),
	const int *qlens : itype(_Ptr<const int>),
    unsigned char *const *answers : itype(_Ptr<_Array_ptr<unsigned char> const>),
    int *alens : itype(_Ptr<int>),
    int asize);

// Add a prototype for __res_mkquery so it can be declared in a checked scope.
hidden int __res_mkquery(int op,
	const char *dname : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	const unsigned char *data : count(datalen),
	int datalen,
	const unsigned char *newrr: count(0), // newrr is unused.
	unsigned char *buf : count(buflen),
	int buflen);


#endif
