#include <string.h>
#include <resolv.h>

/* RFC 1035 message compression */

// The static functions getoffs, getlens and match are only used in this file.
// Use checked pointers rather than bounds-safe interfaces for their parameters.
# pragma CHECKED_SCOPE on

/* label start offsets of a compressed domain name s */
static int getoffs(_Array_ptr<short> offs : count(128), // Fixed-size, allocated in match.
	_Nt_array_ptr<const unsigned char> base : count(0),
	_Nt_array_ptr<const unsigned char> s : count(0))
{
	int i=0;
	for (;;) {
		while (*s & 0xc0) {
			if ((*s & 0xc0) != 0xc0) return 0;
			// At this point s must have length at least 1 (s[0] cannot be 0).
			// Declare s_widened with widened bounds.
			_Nt_array_ptr<const unsigned char> s_widened : count(1) =
				_Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(s, count(1));
			s = base + ((s_widened[0]&0x3f)<<8 | s_widened[1]);
		}
		if (!*s) return i;
		if (s-base >= 0x4000) return 0;
		offs[i++] = s-base;
		s += *s + 1;
	}
}

/* label lengths of an ascii domain name s */
static int getlens(_Array_ptr<unsigned char> lens : count(127), // Fixed-size, allocated in dn_comp.
	_Array_ptr<const char> s : count(l),
	int l)
{
	int i=0,j=0,k=0;
	for (;;) {
		for (; j<l && s[j]!='.'; j++);
		if (j-k-1u > 62) return 0;
		lens[i++] = j-k;
		if (j==l) return i;
		k = ++j;
	}
}

/* longest suffix match of an ascii domain with a compressed domain name dn */
static int match(_Ptr<int> offset,
	_Nt_array_ptr<const unsigned char> base,
	_Nt_array_ptr<const unsigned char> dn,
	_Array_ptr<const char> end,
	_Array_ptr<const unsigned char> lens : count(nlen),
	int nlen)
{
	int l, o, m=0;
	short offs _Checked[128];
	int noff = getoffs(offs, base, dn);
	if (!noff) return 0;
	for (;;) {
		l = lens[--nlen];
		o = offs[--noff];
		end -= l;
		// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
		_Unchecked {
			if (l != base[o] || memcmp((void *)base+o+1, (void *)end, l))
				return m;
		}
		*offset = o;
		m += l;
		if (nlen) m++;
		if (!nlen || !noff) return m;
		end--;
	}
}

_Checked int dn_comp(const char *src : itype(_Nt_array_ptr<const char>),
	unsigned char *dst : count(space - 1) itype(_Nt_array_ptr<unsigned char>),
	int space,
	unsigned char **dnptrs : bounds(dnptrs, lastdnptr) itype(_Array_ptr<_Nt_array_ptr<unsigned char>>),
	unsigned char **lastdnptr : itype(_Array_ptr<_Nt_array_ptr<unsigned char>>))
{
	int i, j, n, m=0, offset, bestlen=0, bestoff;
	unsigned char lens _Checked[127];
	_Array_ptr<_Nt_array_ptr<unsigned char>> p : bounds(dnptrs, lastdnptr) = dnptrs;
	size_t l;
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		l = strnlen(src, 255);
	}
	if (l && src[l-1] == '.') l--;
	if (l>253 || space<=0) return -1;
	if (!l) {
		*dst = 0;
		return 1;
	}
	_Array_ptr<const char> end = src+l;
	n = getlens(lens, src, l);
	if (!n) return -1;

	if (p && *p) for (p++; *p; p++) {
		m = match(&offset, *dnptrs, *p, end, lens, n);
		if (m > bestlen) {
			bestlen = m;
			bestoff = offset;
			if (m == l)
				break;
		}
	}

	/* encode unmatched part */
	if (space < l-bestlen+2+(bestlen-1 < l-1)) return -1;
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		memcpy(dst+1, src, l-bestlen);
	}
	for (i=j=0; i<l-bestlen; i+=lens[j++]+1)
		dst[i] = lens[j];

	/* add tail */
	if (bestlen) {
		dst[i++] = 0xc0 | bestoff>>8;
		dst[i++] = bestoff;
	} else
		dst[i++] = 0;

	/* save dst pointer */
	if (i>2 && lastdnptr && dnptrs && *dnptrs) {
		while (*p) p++;
		if (p+1 < lastdnptr) {
			*p = dst;
			p++;
			*p=0;
		}
	}
	return i;
}
