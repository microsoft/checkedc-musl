#include <resolv.h>
#include <netdb.h>

_Checked int res_query(const char *name : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	unsigned char *dest : count(len),
	int len)
{
	unsigned char q _Checked[280];
	int ql;
	ql = __res_mkquery(0, name, class, type, 0, 0, 0, q, sizeof q);
	_Unchecked {
	}
	if (ql < 0) return ql;
	return __res_send(q, ql, dest, len);
}

weak_alias(res_query, res_search);
