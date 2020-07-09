#include <string.h>
#include <resolv.h>

/* RFC 1035 message compression */

// The static functions getoffs, getlens and match are only used in this file.
// Use checked pointers rather than bounds-safe interfaces for their parameters.
# pragma CHECKED_SCOPE on

// getoffs labels start offsets of a compressed domain name s relative to base, which is
// the start of the domain name lists. The offsets are stored in the buffer offs with a
// max length of 128. Returns the size of offs actually used.
// Null-terminated strings base and s initially have unknown size, i.e., count(0).
static int getoffs(_Array_ptr<short> offs : count(128), // Fixed-size, allocated in match.
	_Nt_array_ptr<const unsigned char> base,
	_Nt_array_ptr<const unsigned char> s)
{
	int i=0;
	for (;;) {
		while (*s & 0xc0) {
			if ((*s & 0xc0) != 0xc0) return 0;

			// (*s & 0xc0) != 0 implies *s != 0. So we can widen the bounds of s.
			// Manually widen the bounds for now. The compiler cannot figure it out yet.
			_Nt_array_ptr<const unsigned char> s_widened : count(1) =
				_Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(s, count(1));
			// Decode offset and calculate the start position of the compressed domain name.
			s = base + ((s_widened[0]&0x3f)<<8 | s_widened[1]);
		}
		// s reaches the end of the domain name.
		if (!*s) return i;

		if (s-base >= 0x4000) return 0;
		offs[i++] = s-base;
		// Move to the next component.
		s += *s + 1;
	}
}

// getlens labels length of each component in an ascii domain name s of length l.
// The lengths are stored in lens of a max size of 127. Returns the actual size of lens.
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

// match matches the longest suffix of an ascii domain with a compressed domain name dn.
// Returns the length of the common suffix and store the offset.
// base and dn initially have count(0) prefixes. end points to the end of the unmatched
// part of the domain name src. The bounds of end are not passed as parameters but recovered
// indirectly from lens. Add parameter src to recover bounds information for end.
static int match(_Ptr<int> offset,
	_Nt_array_ptr<const unsigned char> base,
	_Nt_array_ptr<const unsigned char> dn,
	_Array_ptr<const char> src : bounds(src, end),
	_Array_ptr<const char> end : bounds(src, end),
	_Array_ptr<const unsigned char> lens : count(nlen),
	int nlen)
{
	int l, o, m=0;
	short offs _Checked[128];
	int noff = getoffs(offs, base, dn);
	if (!noff) return 0;
	// Start from the end of the input domain name (src in dn_comp).
	for (;;) {
		l = lens[--nlen];
		o = offs[--noff];
		end -= l;
		// Compare the suffixes. Return if not match.
		if (l != base[o] || memcmp(base+o+1, end, l))
			return m;

		// We matched one component. Save offset and continue to the next.
		*offset = o;
		m += l;
		if (nlen) m++;
		if (!nlen || !noff) return m;
		end--;
	}
}

// dn_comp compresses the domain name src and stores it in the buffer dst of length space. The
// compresssion uses an array of pointers dnptrs to previously compressed names in the current
// message. The first pointer points to the beggining of the message and the list ends with NULL.
// The limit of the array is specified with lastdnptr. If dnptrs is NULL, domain names are not
// compressed. If lastdnptr is NULL, the list of labels is not updated.
int dn_comp(const char *src : itype(_Nt_array_ptr<const char>),
	unsigned char *dst : count(space - 1) itype(_Nt_array_ptr<unsigned char>),
	int space,
	unsigned char **dnptrs : bounds(dnptrs, lastdnptr) itype(_Array_ptr<_Nt_array_ptr<unsigned char>>),
	unsigned char **lastdnptr : count(0) itype(_Array_ptr<_Nt_array_ptr<unsigned char>>))
{
	int i, j, n, m=0, offset, bestlen=0, bestoff;
	unsigned char lens _Checked[127];
	size_t l;
	// TODO(yahsun): remove the unchecked scope once the str* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		l = strnlen(src, 255);
	}
	if (l && src[l-1] == '.') l--;
	if (l>253 || space<=0) return -1;
	if (!l) {
		*dst = 0;
		return 1;
	}
	_Array_ptr<const char> end : bounds(src, end) = src + l;
	n = getlens(lens, src, l);
	// Invariant: l = sum(lens) + n.
	if (!n) return -1;

	_Array_ptr<_Nt_array_ptr<unsigned char>> p : bounds(dnptrs, lastdnptr) = dnptrs;
	if (p && *p) for (p++; *p; p++) {
		m = match(&offset, *dnptrs, *p, src, end, lens, n);
		if (m > bestlen) {
			bestlen = m;
			bestoff = offset;
			if (m == l)
				break;
		}
	}

	/* encode unmatched part */
	if (space < l-bestlen+2+(bestlen-1 < l-1)) return -1;
	memcpy(dst+1, src, l-bestlen);
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
