#include <resolv.h>
#include <string.h>
#include <time.h>

_Checked int __res_mkquery(int op,
	const char *arg_dname : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	const unsigned char *data : count(datalen),
	int datalen,
	const unsigned char *newrr: count(0), // newrr is unused.
	unsigned char *buf : count(buflen),
	int buflen)
{
	int id, i, j;
	unsigned char q _Checked[280];
	struct timespec ts;
	size_t l, bl;
	// TODO: Cleanup the _Assume_bounds_cast once the strlen-based bounds widening is implemented.
	bl = strnlen(arg_dname, 255);
	_Nt_array_ptr<const char> dname : bounds(arg_dname, arg_dname + bl) = 0;
	_Unchecked { dname = _Assume_bounds_cast<_Nt_array_ptr<const char>>(arg_dname, count(bl)); }
	l = bl;
	int n;

	if (l && dname[l-1]=='.') l--;
	n = 17+l+!!l;
	if (l>253 || buflen<n || op>15u || class>255u || type>255u)
		return -1;

	/* Construct query template - ID will be filled later */
	_Array_ptr<unsigned char> arg_q1 : count((size_t)n) =
	                                   _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(q, count((size_t)n));
	memset(arg_q1, 0, n);
	q[2] = op*8 + 1;
	q[3] = 32; /* AD */
	q[5] = 1;
	_Array_ptr<unsigned char> arg_q2 : count(l) = _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(q+13, count(l));
	_Array_ptr<unsigned char> arg_dname1 : count(l) =
	                                       _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(dname, count(l));
	memcpy(arg_q2, arg_dname1, l);
	for (i=13; q[i]; i=j+1) {
		for (j=i; q[j] && q[j] != '.'; j++);
		if (j-i-1u > 62u) return -1;
		q[i-1] = j-i;
	}
	q[i+1] = type;
	q[i+3] = class;

	/* Make a reasonably unpredictable id */
	// clock_gettime has no bounds-safe interface.
	_Unchecked {
		clock_gettime(CLOCK_REALTIME, &ts);
	}
	id = ts.tv_nsec + ts.tv_nsec/65536UL & 0xffff;
	q[0] = id/256;
	q[1] = id;
	_Array_ptr<unsigned char> arg_buf : count((size_t)n) =
	                                    _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(buf, count((size_t)n));
	memcpy(arg_buf, arg_q1, n);
	return n;
}

weak_alias(__res_mkquery, res_mkquery);
