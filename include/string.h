#ifndef	_STRING_H
#define	_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
#define __NEED_locale_t
#endif

#include <bits/alltypes.h>


void *memcpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n), const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n), size_t n) : itype(_Array_ptr<void>) byte_count(n); //return bound needs to be revisited
void *memmove (void * dest: itype(_Array_ptr<void>) byte_count(n), const void * : itype(_Array_ptr<const void>), size_t n) : bounds(dest, (_Array_ptr<char>)dest + n);
void *memset (void * dest: itype(_Array_ptr<void>) byte_count(n), int, size_t n) : itype(_Array_ptr<void>) bounds(dest, (_Array_ptr<char>) dest + n);
int memcmp (const void * : itype(_Array_ptr<const void>) byte_count(n), const void * : itype(_Array_ptr<const void>) byte_count(n), size_t n);
void *memchr (const void *src : itype(_Array_ptr<const void>) byte_count(n), int, size_t n) : itype(_Array_ptr<void>) bounds((_Array_ptr<char>)src, (_Array_ptr<char>) src + n);

char *strcpy (char *__restrict, const char *__restrict);
char *strncpy (char *__restrict, const char *__restrict, size_t);

char *strcat (char *__restrict, const char *__restrict);
char *strncat (char *__restrict, const char *__restrict, size_t);

int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);

int strcoll (const char *, const char *);
size_t strxfrm (char *__restrict, const char *__restrict, size_t);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *__restrict, const char *__restrict);

size_t strlen (const char *);

char *strerror (int);

#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
#include <strings.h>
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
char *strtok_r (char *__restrict, const char *__restrict, char **__restrict);
int strerror_r (int, char *, size_t);
char *stpcpy(char *__restrict, const char *__restrict);
char *stpncpy(char *__restrict, const char *__restrict, size_t);
size_t strnlen (const char *, size_t);
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
void *memccpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n), const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n), int,  size_t n) : itype(_Array_ptr<void>) byte_count(n); //return bound needs to be revisited
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char *strsep(char **, const char *);
size_t strlcat (char *, const char *, size_t);
size_t strlcpy (char *, const char *, size_t);
void explicit_bzero (void *, size_t);
#endif

#ifdef _GNU_SOURCE
#define	strdupa(x)	strcpy(alloca(strlen(x)+1),x)
int strverscmp (const char *, const char *);
char *strchrnul(const char *, int);
char *strcasestr(const char *, const char *);
void *memmem(const void * : itype(_Array_ptr<const void>) byte_count(k), size_t k, const void * : itype(_Array_ptr<const void>) byte_count(l), size_t l) : itype(_Array_ptr<void>) byte_count(k);
void *memrchr(const void * : itype(_Array_ptr<const void>) byte_count(n), int, size_t n) : itype(_Array_ptr<void>) byte_count(n);
void *mempcpy(void * : itype(_Array_ptr<void>) byte_count(n), const void * : itype(_Array_ptr<const void>) byte_count(n), size_t n) : itype(_Array_ptr<void>);
#ifndef __cplusplus
char *basename();
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
