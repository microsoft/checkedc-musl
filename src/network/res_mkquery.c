#include <resolv.h>
#include <string.h>
#include <time.h>

_Checked int __res_mkquery(int op,
	const char *dname : itype(_Nt_array_ptr<const char>),
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
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	size_t l;
	_Unchecked {
		l = strnlen(dname, 255);
	}
	int n;

	if (l && dname[l-1]=='.') l--;
	n = 17+l+!!l;
	if (l>253 || buflen<n || op>15u || class>255u || type>255u)
		return -1;

	/* Construct query template - ID will be filled later */
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		memset((void *)q, 0, n);
	}
	q[2] = op*8 + 1;
	q[3] = 32; /* AD */
	q[5] = 1;
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		memcpy((char *)q+13, dname, l);
	}
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
	// TODO(yahsun): remove the unchecked scope once the str* and mem* functions are annotated with bounds-safe interfaces.
	_Unchecked {
		memcpy(buf, (void *)q, n);
	}
	return n;
}

weak_alias(__res_mkquery, res_mkquery);
