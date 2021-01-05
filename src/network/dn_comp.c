#include <string.h>
#include <resolv.h>

/* RFC 1035 message compression */

// The static functions getoffs, getlens and match are only used in this file.
// Use checked pointers rather than bounds-safe interfaces for their parameters.
# pragma CHECKED_SCOPE on

// getoffs labels start offsets of a compressed domain name s relative to base, which is
// the start of the domain name lists. The offsets are stored in the buffer offs with a
// max length of 128. Returns the size of offs actually used.
static int getoffs(_Array_ptr<short> offs : count(128), // Fixed-size, allocated in match.
	_Nt_array_ptr<const unsigned char> arg_base,
	_Nt_array_ptr<const unsigned char> arg_s)
{
	int i=0;
	// TODO: Cleanup the _Assume_bounds_cast once the strlen-based bounds widening is implemented.
	size_t len_s = strlen((_Nt_array_ptr<const char>)arg_s);
	_Nt_array_ptr<const unsigned char> s : bounds(arg_s, arg_s + len_s) = 0;
	_Nt_array_ptr<const unsigned char> base : bounds(arg_s, arg_s + len_s) = 0;
	_Unchecked {
		s = _Assume_bounds_cast<_Nt_array_ptr<const unsigned char>>(arg_s, bounds(arg_s, arg_s + len_s));
		base = _Assume_bounds_cast<_Nt_array_ptr<const unsigned char>>
		                                        (arg_base, bounds(arg_s, arg_s + len_s));
	}
	for (;;) {
		while (*s & 0xc0) {
			if ((*s & 0xc0) != 0xc0) return 0;
			s = _Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>
			                        (base + ((s[0]&0x3f)<<8 | s[1]), bounds(arg_s, arg_s + len_s));
		}
		if (!*s) return i;
		if (s-base >= 0x4000) return 0;
		offs[i++] = s-base;
		s += *s + 1;
	}
}

// getlens labels length of each component in an ascii domain name s of length l.
// The lengths are stored in lens of a max size of 127. Returns the actual size of lens.
static int getlens(_Array_ptr<unsigned char> lens : count(127), // Fixed-size, allocated in dn_comp.
	_Array_ptr<const char> s : count(l),
	size_t l)
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
	_Nt_array_ptr<const unsigned char> arg_base,
	_Nt_array_ptr<const unsigned char> dn,
	_Array_ptr<const char> arg_src : bounds(arg_src, arg_end),
	_Array_ptr<const char> arg_end : bounds(arg_src, arg_end),
	_Array_ptr<const unsigned char> lens : count(arg_nlen),
	const int arg_nlen)
{
	int l, o, m=0;
	short offs _Checked[128];
	int noff = getoffs(offs, arg_base, dn);
	if (!noff) return 0;

	// Start from the end of the input domain name (src in dn_comp).
	int nlen = arg_nlen;
	_Array_ptr<const char> end : bounds(arg_src, arg_end) = arg_end;
	// TODO: Cleanup the _Assume_bounds_cast once the strlen-based bounds widening is implemented.
	size_t len_base = strlen((_Nt_array_ptr<const char>)arg_base);
	_Nt_array_ptr<const unsigned char> base : bounds(arg_base, arg_base + len_base) = 0;
	_Unchecked { base = _Assume_bounds_cast<_Nt_array_ptr<const unsigned char>>
	                                       (arg_base, bounds(arg_base, arg_base + len_base)); }
	for (;;) {
		l = lens[--nlen];
		o = offs[--noff];
		end -= l;

		size_t arg_l = l;
		_Nt_array_ptr<const unsigned char> arg_base_1 : count(arg_l) =
		         _Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(base+o+1, count(arg_l));
		_Array_ptr<const char> arg_end_1 : count(arg_l) =
		         _Dynamic_bounds_cast<_Array_ptr<const char>>(end, count(arg_l));

		// Compare the suffixes. Return if not match.
		if (l != base[o] || memcmp(arg_base_1, arg_end_1, arg_l))
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
// message. The first pointer points to the beginning of the message and the list ends with NULL.
// The limit of the array is specified with lastdnptr. If dnptrs is NULL, domain names are not
// compressed. If lastdnptr is NULL, the list of labels is not updated.
int dn_comp(const char *arg_src : itype(_Nt_array_ptr<const char>),
	unsigned char *dst : count(space - 1) itype(_Nt_array_ptr<unsigned char>),
	int space,
	unsigned char **dnptrs : bounds(dnptrs, lastdnptr) itype(_Array_ptr<_Nt_array_ptr<unsigned char>>),
	unsigned char **lastdnptr : count(0) itype(_Array_ptr<_Nt_array_ptr<unsigned char>>))
{
	int i, j, n, m=0, offset, bestlen=0, bestoff;
	unsigned char lens _Checked[127];
	size_t l;
	_Unchecked { l = strnlen((const char *)arg_src, 255); }
	_Nt_array_ptr<const char> src : count(l) = 0;
	_Unchecked { src = _Assume_bounds_cast<_Nt_array_ptr<const char>>(arg_src, count(l)); }
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

	_Array_ptr<unsigned char> arg_lens : count(n) =
	                                     _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(lens, count(n));
	_Array_ptr<_Nt_array_ptr<unsigned char>> p : bounds(dnptrs, lastdnptr) = dnptrs;
	if (p && *p) for (p++; *p; p++) {
		m = match(&offset, *dnptrs, *p, src, end, arg_lens, n);
		if (m > bestlen) {
			bestlen = m;
			bestoff = offset;
			if (m == l)
				break;
		}
	}

	/* encode unmatched part */
	if (space < l-bestlen+2+(bestlen-1 < l-1)) return -1;
	_Nt_array_ptr<unsigned char> arg_dst : count(l - bestlen) =
	         _Dynamic_bounds_cast<_Nt_array_ptr<unsigned char>>(dst + 1, count(l - bestlen));
	_Nt_array_ptr<const char> arg_src_1 : count(l - bestlen) =
	         _Dynamic_bounds_cast<_Nt_array_ptr<const char>>(src, count(l - bestlen));
	memcpy(arg_dst, arg_src_1, l-bestlen);
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
			_Nt_array_ptr<unsigned char> arg_dst_1 : count(0) =
	         		_Dynamic_bounds_cast<_Nt_array_ptr<unsigned char>>(dst, count(0));
			*p = arg_dst_1;
			p++;
			*p=0;
		}
	}
	return i;
}
