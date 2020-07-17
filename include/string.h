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

// Returns a pointer to dest.
// Copies n bytes form src to the memory region dest points to.
void *memcpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n),
              const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n),
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
// Returns a pointer to dest.
// Copies n bytes from memory area src to memory area dest. The memory areas may overlap.
void *memmove (void *dest: itype(_Array_ptr<void>) byte_count(n),
               const void * : itype(_Array_ptr<const void>) byte_count(n),
               size_t n)
  : byte_count(n);
// Returns a pointer to dest.
// Fills the first n bytes of the memory area pointed to by dest with the constant byte c.
void *memset (void *dest: itype(_Array_ptr<void>) byte_count(n),
              int c,
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
// Compares the first n bytes of the memory areas s1 and s2.
int memcmp (const void * : itype(_Array_ptr<const void>) byte_count(n),
            const void * : itype(_Array_ptr<const void>) byte_count(n),
            size_t n);
// Returns a pointer to the matching byte.
// Scans the initial n bytes of the memory area pointed to by src for the first instance of c.
void *memchr (const void *src : itype(_Array_ptr<const void>) byte_count(n),
              int c,
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
char *strcpy (char *__restrict, const char *__restrict);
// Copies the first n characters of source to destination. If the end of the source C string (which is signaled by a null-character) is found before num characters have been copied, destination is padded with zeros until a total of num characters have been written to it.
char *strncpy(char *__restrict d : itype(__restrict _Nt_array_ptr<char>) count(n),
              const char *__restrict s : itype(__restrict _Nt_array_ptr<const char>) count(n),
              size_t n)
  : itype(_Nt_array_ptr<char>) count(n);

char *strcat (char *__restrict, const char *__restrict);
char *strncat (char *__restrict, const char *__restrict, size_t);

int strcmp (const char *, const char *);
// Compares up to n characters of the C string str1 to those of the C string str2.
int strncmp(const char * : itype(_Nt_array_ptr<const char>) count(n),
            const char * : itype(_Nt_array_ptr<const char>) count(n),
            size_t n);

int strcoll (const char *, const char *);
size_t strxfrm (char *__restrict, const char *__restrict, size_t);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *__restrict, const char *__restrict);
// Returns the length of string.
size_t strlen (const char * : itype(_Array_ptr<const char>));

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
// Copies at most n characters from the string pointed to by src, including the terminating null byte ('\0'),
// to the array pointed to by dest.
char *stpncpy(char *restrict d : itype(restrict _Nt_array_ptr<char>) count(n),
              const char *restrict s : itype(restrict _Nt_array_ptr<const char>) count(n),
              size_t n)
  :itype(_Nt_array_ptr<char>) count(n);
// Returns the number of bytes in the string pointed to by s, excluding the terminating null byte ('\0').
size_t strnlen (const char * : itype(_Array_ptr<const char>) count(n), size_t n);
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
// Returns a pointer to the next character in dest after c.
// Copies no more than n bytes from memory area src to memory area dest, stopping when the character c is found.
void *memccpy (void *__restrict dest : itype(__restrict _Array_ptr<void>) byte_count(n),
               const void *__restrict src : itype(__restrict _Array_ptr<const void>) byte_count(n),
               int c,
               size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char *strsep(char **, const char *);
//size_t strlcat (char *, const char *, size_t);
// Appends the NUL-terminated string s to the end of d.
// It will append at most n - strlen(dst) - 1 bytes, NUL-terminating the result.
size_t strlcat(char *d : itype(_Nt_array_ptr<char>) count(n),
               const char *s : itype(_Nt_array_ptr<const char>) count(n),
               size_t n);
// Copies up to size - 1 characters from the NUL-terminated string s to d,
// NUL-terminating the result.
size_t strlcpy(char *d : itype(_Nt_array_ptr<char>) count(n),
               const char *s : itype(_Nt_array_ptr<const char>) count(n),
               size_t n);
// Places n zero-valued bytes in the area pointed to by s.
// Will not be removed by a compiler's dead store optimization pass
void explicit_bzero(void *d : itype(_Array_ptr<void>) byte_count(n), size_t n);
#endif

#ifdef _GNU_SOURCE
#define	strdupa(x)	strcpy(alloca(strlen(x)+1),x)
int strverscmp (const char *, const char *);
char *strchrnul(const char *, int);
char *strcasestr(const char *, const char *);
// Converting memmem() function to checked version causes unexpected
// illegal instruction for some test cases. See issue #868
// (https://github.com/microsoft/checkedc-clang/issues/868)
void *memmem(const void *, size_t, const void *, size_t);
// Returns a pointer to the matching byte.
// Searches backward from the end of the n bytes pointed to by m
// for the first instance of c.
void *memrchr(const void *m : itype(_Array_ptr<const void>) byte_count(n),
              int c,
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
// Returns a pointer to the byte following the last written byte.
// Copies n bytes from the object beginning at src into the object pointed to by dest.
void *mempcpy(void *dest : itype(_Array_ptr<void>) byte_count(n),
              const void *src : itype(_Array_ptr<const void>) byte_count(n),
              size_t n)
  : itype(_Array_ptr<void>) byte_count(n);
#ifndef __cplusplus
char *basename();
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
