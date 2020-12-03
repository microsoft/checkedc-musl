#define _BSD_SOURCE
#include <errno.h>
#include <stddef.h>
#include <resolv.h>
#include <arpa/nameser.h>

const struct _ns_flagdata _ns_flagdata[16] = {
	{ 0x8000, 15 },
	{ 0x7800, 11 },
	{ 0x0400, 10 },
	{ 0x0200, 9 },
	{ 0x0100, 8 },
	{ 0x0080, 7 },
	{ 0x0040, 6 },
	{ 0x0020, 5 },
	{ 0x0010, 4 },
	{ 0x000f, 0 },
	{ 0x0000, 0 },
	{ 0x0000, 0 },
	{ 0x0000, 0 },
	{ 0x0000, 0 },
	{ 0x0000, 0 },
	{ 0x0000, 0 },
};

_Checked unsigned ns_get16(const unsigned char *cp : count(2))
{
	return cp[0]<<8 | cp[1];
}

_Checked unsigned long ns_get32(const unsigned char *cp : count(4))
{
	return (unsigned)cp[0]<<24 | cp[1]<<16 | cp[2]<<8 | cp[3];
}

_Checked void ns_put16(unsigned s, unsigned char *arg_cp : count(2))
{
	_Array_ptr<unsigned char> cp : bounds(arg_cp, arg_cp + 2) = arg_cp;
	*cp = s>>8;
	cp++;
	*cp = s;
	cp++;
}

_Checked void ns_put32(unsigned long l, unsigned char *arg_cp : count(4))
{
	_Array_ptr<unsigned char> cp : bounds(arg_cp, arg_cp + 4) = arg_cp;
	*cp = l>>24;
	cp++;
	*cp = l>>16;
	cp++;
	*cp = l>>8;
	cp++;
	*cp = l;
	cp++;
}

_Checked int ns_initparse(const unsigned char *arg_msg : count(msglen),
	int msglen,
	ns_msg *handle : itype(_Ptr<ns_msg>))
{
	int i, r;

	_Array_ptr<const unsigned char> msg : bounds(arg_msg, arg_msg + msglen) = arg_msg;
	handle->_eom = msg + msglen;
	handle->_msg = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(msg, bounds(handle->_msg, handle->_eom));
	if (msglen < (2 + ns_s_max) * NS_INT16SZ) goto bad;
	// Invariant: msglen >= (2 + ns_s_max) * NS_INT16SZ.
	_Array_ptr<const unsigned char> arg_msg_1 : count (2) = 0;
	arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(msg, count(2));
	NS_GET16(handle->_id, arg_msg_1, msg);
	arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(msg, count(2));
	NS_GET16(handle->_flags, arg_msg_1, msg);
	for (i = 0; i < ns_s_max; i++) {
		arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(msg, count(2));
		NS_GET16(handle->_counts[i], arg_msg_1, msg);
	}
	for (i = 0; i < ns_s_max; i++) {
		if (handle->_counts[i]) {
			handle->_sections[i] = msg;
			_Array_ptr<const unsigned char> arg_msg_2 : bounds(arg_msg_2, handle->_eom) = 
			_Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(msg, bounds(msg, handle->_eom));
			r = ns_skiprr(arg_msg_2, handle->_eom, i, handle->_counts[i]);
			if (r < 0) return -1;
			msg += r;
		} else {
			handle->_sections[i] = NULL;
		}
	}
	if (msg != handle->_eom) goto bad;
	handle->_sect = ns_s_max;
	handle->_rrnum = -1;
	handle->_msg_ptr = NULL;
	return 0;
bad:
	errno = EMSGSIZE;
	return -1;
}

_Checked int ns_skiprr(const unsigned char *ptr: bounds(ptr, eom),
	const unsigned char *eom : itype(_Array_ptr<const unsigned char>),
	ns_sect section,
	int count)
{
	_Array_ptr<const unsigned char> p : bounds(ptr, eom) = ptr;
	int r;

	_Array_ptr<const unsigned char> arg_p : count (2) = 0;
	while (count--) {
		r = dn_skipname(p, eom);
		if (r < 0) goto bad;
		if (r + 2 * NS_INT16SZ > eom - p) goto bad;
		p += r + 2 * NS_INT16SZ;
		if (section != ns_s_qd) {
			if (NS_INT32SZ + NS_INT16SZ > eom - p) goto bad;
			p += NS_INT32SZ;
			arg_p = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(p, count(2));
			NS_GET16(r, arg_p, p);
			if (r > eom - p) goto bad;
			p += r;
		}
	}
	return p - ptr;
bad:
	errno = EMSGSIZE;
	return -1;
}

_Checked int ns_parserr(ns_msg *handle : itype(_Ptr<ns_msg>),
	ns_sect section,
	int rrnum,
	ns_rr *rr : itype(_Ptr<ns_rr>))
{
	int r;

	if (section < 0 || section >= ns_s_max) goto bad;
	if (section != handle->_sect) {
		handle->_sect = section;
		handle->_rrnum = 0;
		// The assignment is not allowed in a checked scope because RHS has unknown bounds. We cannot declare
		// bounds for RHS currently in Checked C. See the comments in nameser.h.
		_Unchecked {
			handle->_msg_ptr = handle->_sections[section];
		}
	}
	if (rrnum == -1) rrnum = handle->_rrnum;
	if (rrnum < 0 || rrnum >= handle->_counts[section]) goto bad;
	if (rrnum < handle->_rrnum) {
		handle->_rrnum = 0;
		// The assignment is not allowed in a checked scope because RHS has unknown bounds. We cannot declare
		// bounds for RHS currently in Checked C. See the comments in nameser.h.
		_Unchecked {
			handle->_msg_ptr = handle->_sections[section];
		}
	}
	if (rrnum > handle->_rrnum) {
		_Array_ptr<const unsigned char> arg_msg_ptr : bounds(arg_msg_ptr, handle->_eom) = 
		_Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(handle->_msg_ptr,
		                                                      bounds(handle->_msg_ptr, handle->_eom));
		r = ns_skiprr(arg_msg_ptr, handle->_eom, section, rrnum - handle->_rrnum);
		if (r < 0) return -1;
		handle->_msg_ptr += r;
		handle->_rrnum = rrnum;
	}
	r = ns_name_uncompress(handle->_msg, handle->_eom, handle->_msg_ptr, rr->name, NS_MAXDNAME);
	if (r < 0) return -1;
	handle->_msg_ptr += r;
	if (2 * NS_INT16SZ > handle->_eom - handle->_msg_ptr) goto size;
	_Array_ptr<const unsigned char> arg_msg_1 : count (2) = 0;
	_Array_ptr<const unsigned char> arg_msg_2 : count (4) = 0;
	arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(handle->_msg_ptr, count(2));
	NS_GET16(rr->type, arg_msg_1, handle->_msg_ptr);
	arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(handle->_msg_ptr, count(2));
	NS_GET16(rr->rr_class, arg_msg_1, handle->_msg_ptr);
	if (section != ns_s_qd) {
		if (NS_INT32SZ + NS_INT16SZ > handle->_eom - handle->_msg_ptr) goto size;
		arg_msg_2 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(handle->_msg_ptr, count(4));
		NS_GET32(rr->ttl, arg_msg_2, handle->_msg_ptr);
		arg_msg_1 = _Dynamic_bounds_cast<_Array_ptr<const unsigned char>>(handle->_msg_ptr, count(2));
		NS_GET16(rr->rdlength, arg_msg_1, handle->_msg_ptr);
		if (rr->rdlength > handle->_eom - handle->_msg_ptr) goto size;
		rr->rdata = handle->_msg_ptr;
		handle->_msg_ptr += rr->rdlength;
	} else {
		rr->ttl = 0;
		rr->rdlength = 0;
		rr->rdata = NULL;
	}
	handle->_rrnum++;
	if (handle->_rrnum > handle->_counts[section]) {
		handle->_sect = section + 1;
		if (handle->_sect == ns_s_max) {
			handle->_rrnum = -1;
			handle->_msg_ptr = NULL;
		} else {
			handle->_rrnum = 0;
		}
	}
	return 0;
bad:
	errno = ENODEV;
	return -1;
size:
	errno = EMSGSIZE;
	return -1;
}

_Checked int ns_name_uncompress(const unsigned char *msg : bounds(msg, eom),
	const unsigned char *eom : itype(_Array_ptr<const unsigned char>),
	const unsigned char *src : bounds(msg, eom),
	char *dst : count(dstsiz) itype(_Array_ptr<char>),
	size_t dstsiz)
{
	int r;
	_Array_ptr<char> arg_dst : bounds (arg_dst, arg_dst + (int)dstsiz) =
	        _Dynamic_bounds_cast<_Array_ptr<char>>(dst, bounds(dst, dst + (int)dstsiz));
	r = dn_expand(msg, eom, src, arg_dst, dstsiz);
	if (r < 0) errno = EMSGSIZE;
	return r;
}

