#include <resolv.h>
#include <netdb.h>

_Checked int res_query(const char *name : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	unsigned char *dest : count(len),
	int len)
{
	unsigned char q _Checked[280];
	int ql = __res_mkquery(0, name, class, type, 0, 0, 0, q, sizeof q);
	if (ql < 0) return ql;
	_Array_ptr<unsigned char> arg_q : count(ql) =
	                                  _Dynamic_bounds_cast<_Array_ptr<unsigned char>>(q, count(ql));
	return __res_send(arg_q, ql, dest, len);
}

weak_alias(res_query, res_search);
