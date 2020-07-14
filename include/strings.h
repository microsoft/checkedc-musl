#ifndef	_STRINGS_H
#define	_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif


#define __NEED_size_t
#define __NEED_locale_t
#include <bits/alltypes.h>

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_POSIX_SOURCE) \
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE+0 < 200809L) \
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE+0 < 700)
// Compares the two byte sequences s1 and s2 of length n each.
int bcmp (const void * : itype(_Array_ptr<const void>) byte_count(n),
          const void * : itype(_Array_ptr<const void>) byte_count(n),
          size_t n);
// Copies n bytes from the area pointed to by s1 to the area pointed to by s2.
void bcopy (const void * : itype(_Array_ptr<const void>) byte_count(n),
            const void * : itype(_Array_ptr<const void>) byte_count(n),
            size_t n);
// Places n zero-valued bytes in the area pointed to by s.
void bzero (void * : itype(_Array_ptr<void>) byte_count(n), size_t n);
char *index (const char *, int);
char *rindex (const char *, int);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)  || defined(_BSD_SOURCE)
int ffs (int);
int ffsl (long);
int ffsll (long long);
#endif

int strcasecmp (const char *, const char *);
int strncasecmp (const char *, const char *, size_t);

int strcasecmp_l (const char *, const char *, locale_t);
int strncasecmp_l (const char *, const char *, size_t, locale_t);

#ifdef __cplusplus
}
#endif

#endif
