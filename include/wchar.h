#ifndef _WCHAR_H
#define _WCHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_FILE
#define __NEED___isoc_va_list
#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_wint_t
#define __NEED_mbstate_t

#if __STDC_VERSION__ < 201112L
#define __NEED_struct__IO_FILE
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_locale_t
#define __NEED_va_list
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_wctype_t
#endif

#include <bits/alltypes.h>

#if L'\0'-1 > 0
#define WCHAR_MAX (0xffffffffu+L'\0')
#define WCHAR_MIN (0+L'\0')
#else
#define WCHAR_MAX (0x7fffffff+L'\0')
#define WCHAR_MIN (-1-0x7fffffff+L'\0')
#endif

#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#undef WEOF
#define WEOF 0xffffffffU

wchar_t *wcscpy (wchar_t *__restrict, const wchar_t *__restrict);
// Copies the first n characters of source to destination. If the end of the source C wide string is found before n characters have been copied,
// destination is padded with additional null wide characters until a total of n characters have been written to it.
wchar_t *wcsncpy(wchar_t *restrict arg_d : itype(restrict _Nt_array_ptr<wchar_t>)
                                           bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                                  (_Nt_array_ptr<wchar_t>)arg_d + arg_n),
                 const wchar_t *restrict s,
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n);
wchar_t *wcscat (wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcsncat (wchar_t *__restrict, const wchar_t *__restrict, size_t);

int wcscmp (const wchar_t *, const wchar_t *);
// Compares up to n characters of the C wide string l to those of the C wide string r.
int wcsncmp(const wchar_t *l, const wchar_t *r, size_t n);

int wcscoll(const wchar_t *, const wchar_t *);
size_t wcsxfrm (wchar_t *__restrict, const wchar_t *__restrict, size_t);

wchar_t *wcschr (const wchar_t *, wchar_t);
wchar_t *wcsrchr (const wchar_t *, wchar_t);

size_t wcscspn (const wchar_t *, const wchar_t *);
size_t wcsspn (const wchar_t *, const wchar_t *);
wchar_t *wcspbrk (const wchar_t *, const wchar_t *);

wchar_t *wcstok (wchar_t *__restrict, const wchar_t *__restrict, wchar_t **__restrict);

size_t wcslen (const wchar_t *);

wchar_t *wcsstr (const wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcswcs (const wchar_t *, const wchar_t *);

// Searches within the first n wide characters of the block pointed by s for
// the first occurrence of c, and returns a pointer to it.
wchar_t *wmemchr(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(n),
                 wchar_t c,
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n);
// Compares the first n wide characters of the block of memory pointed by l to the first n wide characters pointed by r,
// returning zero if they all match or a value different from zero representing which is greater if they do not.
int wmemcmp(const wchar_t *l, const wchar_t *r, size_t n);
// Copies the values of n elements of type wchar_t from the location pointed by s to the location pointed by d.
wchar_t *wmemcpy(wchar_t *restrict arg_d : itype(restrict _Nt_array_ptr<wchar_t>)
                                           bounds((_Nt_array_ptr<wchar_t>)arg_d,
                                                  (_Nt_array_ptr<wchar_t>)arg_d + arg_n),
                 const wchar_t *restrict arg_s : itype(restrict _Nt_array_ptr<const wchar_t>)
                                                 bounds((_Nt_array_ptr<const wchar_t>)arg_s,
                                                        (_Nt_array_ptr<const wchar_t>)arg_s + arg_n),
                 size_t arg_n)
  : itype(_Nt_array_ptr<wchar_t>) count(arg_n);
// Copies the values of n elements of type wchar_t from the location pointed by s to the location pointed by d.
// Copying takes place as if an intermediate buffer were used, allowing the destination and source to overlap.
wchar_t *wmemmove(wchar_t *d : itype(_Nt_array_ptr<wchar_t>) count(n),
                  const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(n),
                  size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n);
// Sets the first n elements of the array of wide characters pointed by d to the value specified as c.
wchar_t *wmemset(wchar_t *d : itype(_Nt_array_ptr<wchar_t>) count(n),
                 wchar_t c,
                 size_t n)
  : itype(_Nt_array_ptr<wchar_t>) count(n);

wint_t btowc (int);
int wctob (wint_t);

int mbsinit (const mbstate_t *);
size_t mbrtowc (wchar_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t wcrtomb (char *__restrict, wchar_t, mbstate_t *__restrict);

size_t mbrlen (const char *__restrict, size_t, mbstate_t *__restrict);

size_t mbsrtowcs (wchar_t *__restrict, const char **__restrict, size_t, mbstate_t *__restrict);
size_t wcsrtombs (char *__restrict, const wchar_t **__restrict, size_t, mbstate_t *__restrict);

float wcstof (const wchar_t *__restrict, wchar_t **__restrict);
double wcstod (const wchar_t *__restrict, wchar_t **__restrict);
long double wcstold (const wchar_t *__restrict, wchar_t **__restrict);

long wcstol (const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long wcstoul (const wchar_t *__restrict, wchar_t **__restrict, int);

long long wcstoll (const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long long wcstoull (const wchar_t *__restrict, wchar_t **__restrict, int);



int fwide (FILE *, int);


int wprintf (const wchar_t *__restrict, ...);
int fwprintf (FILE *__restrict, const wchar_t *__restrict, ...);
int swprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);

int vwprintf (const wchar_t *__restrict, __isoc_va_list);
int vfwprintf (FILE *__restrict, const wchar_t *__restrict, __isoc_va_list);
int vswprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, __isoc_va_list);

int wscanf (const wchar_t *__restrict, ...);
int fwscanf (FILE *__restrict, const wchar_t *__restrict, ...);
int swscanf (const wchar_t *__restrict, const wchar_t *__restrict, ...);

int vwscanf (const wchar_t *__restrict, __isoc_va_list);
int vfwscanf (FILE *__restrict, const wchar_t *__restrict, __isoc_va_list);
int vswscanf (const wchar_t *__restrict, const wchar_t *__restrict, __isoc_va_list);

wint_t fgetwc (FILE *);
wint_t getwc (FILE *);
wint_t getwchar (void);

wint_t fputwc (wchar_t, FILE *);
wint_t putwc (wchar_t, FILE *);
wint_t putwchar (wchar_t);

wchar_t *fgetws (wchar_t *__restrict, int, FILE *__restrict);
int fputws (const wchar_t *__restrict, FILE *__restrict);

wint_t ungetwc (wint_t, FILE *);

struct tm;
size_t wcsftime (wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict);

#undef iswdigit

#if defined(_GNU_SOURCE)
wint_t fgetwc_unlocked (FILE *);
wint_t getwc_unlocked (FILE *);
wint_t getwchar_unlocked (void);
wint_t fputwc_unlocked (wchar_t, FILE *);
wint_t putwc_unlocked (wchar_t, FILE *);
wint_t putwchar_unlocked (wchar_t);
wchar_t *fgetws_unlocked (wchar_t *__restrict, int, FILE *__restrict);
int fputws_unlocked (const wchar_t *__restrict, FILE *__restrict);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
size_t wcsftime_l (wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict, locale_t);
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)  || defined(_BSD_SOURCE)
FILE *open_wmemstream(wchar_t **, size_t *);
size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t, mbstate_t *__restrict);
size_t wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t, mbstate_t *__restrict);
wchar_t *wcsdup(const wchar_t *);
// Returns the length of the C wide string s.
size_t wcsnlen(const wchar_t *s : itype(_Nt_array_ptr<const wchar_t>) count(arg_n), size_t arg_n);
wchar_t *wcpcpy (wchar_t *__restrict, const wchar_t *__restrict);
// Copies at most n wide characters from the wide-character string pointed to by s, including the terminating null wide ('\0'), to the array pointed to by d
wchar_t *wcpncpy(wchar_t *restrict d : itype(restrict _Array_ptr<wchar_t>)
                                       bounds((_Array_ptr<wchar_t>)d, (_Array_ptr<wchar_t>)d + n),
                 const wchar_t *restrict s,
                 size_t n)
  : itype(_Array_ptr<wchar_t>) count(n);
int wcscasecmp(const wchar_t *, const wchar_t *);
int wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t);
int wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
int wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t);
int wcscoll_l(const wchar_t *, const wchar_t *, locale_t);
size_t wcsxfrm_l(wchar_t *__restrict, const wchar_t *__restrict, size_t, locale_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int wcwidth (wchar_t);
int wcswidth (const wchar_t *, size_t);
int       iswalnum(wint_t);
int       iswalpha(wint_t);
int       iswblank(wint_t);
int       iswcntrl(wint_t);
int       iswdigit(wint_t);
int       iswgraph(wint_t);
int       iswlower(wint_t);
int       iswprint(wint_t);
int       iswpunct(wint_t);
int       iswspace(wint_t);
int       iswupper(wint_t);
int       iswxdigit(wint_t);
int       iswctype(wint_t, wctype_t);
wint_t    towlower(wint_t);
wint_t    towupper(wint_t);
wctype_t  wctype(const char *);

#ifndef __cplusplus
#undef iswdigit
#define iswdigit(a) (0 ? iswdigit(a) : ((unsigned)(a)-'0') < 10)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
