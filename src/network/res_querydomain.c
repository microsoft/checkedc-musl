#include <resolv.h>
#include <string.h>

_Checked int res_querydomain(const char *arg_name : itype(_Nt_array_ptr<const char>),
	const char *arg_domain : itype(_Nt_array_ptr<const char>),
	int class,
	int type,
	unsigned char *dest : count(len),
	int len)
{
	char tmp _Nt_checked[255];
	size_t nl, dl;
	_Unchecked {
		nl = strnlen(arg_name, 255);
		dl = strnlen(arg_domain, 255);
	}
	_Nt_array_ptr<const char> name : bounds(arg_name, arg_name + nl) = 0;
	_Unchecked { name = _Assume_bounds_cast<_Nt_array_ptr<const char>>(arg_name, count(nl)); }
	_Nt_array_ptr<const char> domain : bounds(arg_domain, arg_domain + dl) = 0;
	_Unchecked { domain = _Assume_bounds_cast<_Nt_array_ptr<const char>>(arg_domain, count(dl)); }
	if (nl+dl+1 > 254) return -1;
	_Nt_array_ptr<char> arg_tmp1 : count(nl) = _Dynamic_bounds_cast<_Nt_array_ptr<char>>(tmp, count(nl));
	memcpy(arg_tmp1, name, nl);
	tmp[nl] = '.';
	_Nt_array_ptr<char> arg_tmp2 : count(dl+1) = _Dynamic_bounds_cast<_Nt_array_ptr<char>>(tmp+nl+1, count(dl+1));
	memcpy(arg_tmp2, domain, dl+1);
	return res_query(tmp, class, type, dest, len);
}
