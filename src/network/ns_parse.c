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

_Checked void ns_put16(unsigned s, unsigned char *cp : count(2))
{
	*cp = s>>8;
	cp++;
	*cp = s;
	cp++;
}

_Checked void ns_put32(unsigned long l, unsigned char *cp : count(4))
{
	*cp = l>>24;
	cp++;
	*cp = l>>16;
	cp++;
	*cp = l>>8;
	cp++;
	*cp = l;
	cp++;
}

_Checked int ns_initparse(const unsigned char *msg : count(msglen),
	int msglen,
	ns_msg *handle : itype(_Ptr<ns_msg>))
{
	int i, r;

	// Temporarily use an unchecked block to mask a compiler warning on inability to prove that the inferred
	// bounds(msg, handle->_eom) implies bounds(msg, msg + msglen) on the assignment to handle->_msg.
	// TODO(yahsun): the compiler should be able to prove the above bounds invariant when the two assignments
	// are in a bundled block. So the warning(s) should disapear once bundled blocks are implemented.
	_Unchecked {
		handle->_msg = msg;
		handle->_eom = msg + msglen;
	}

	if (msglen < (2 + ns_s_max) * NS_INT16SZ) goto bad;
	// Invariant: msglen >= (2 + ns_s_max) * NS_INT16SZ.
	NS_GET16(handle->_id, msg);
	NS_GET16(handle->_flags, msg);
	for (i = 0; i < ns_s_max; i++) NS_GET16(handle->_counts[i], msg);
	for (i = 0; i < ns_s_max; i++) {
		if (handle->_counts[i]) {
			handle->_sections[i] = msg;
			r = ns_skiprr(msg, handle->_eom, i, handle->_counts[i]);
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
	const unsigned char *eom : itype(_Ptr<const unsigned char>),
	ns_sect section,
	int count)
{
	_Array_ptr<const unsigned char> p : bounds(ptr, eom) = ptr;
	int r;

	while (count--) {
		r = dn_skipname(p, eom);
		if (r < 0) goto bad;
		if (r + 2 * NS_INT16SZ > eom - p) goto bad;
		p += r + 2 * NS_INT16SZ;
		if (section != ns_s_qd) {
			if (NS_INT32SZ + NS_INT16SZ > eom - p) goto bad;
			p += NS_INT32SZ;
			NS_GET16(r, p);
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
		r = ns_skiprr(handle->_msg_ptr, handle->_eom, section, rrnum - handle->_rrnum);
		if (r < 0) return -1;
		handle->_msg_ptr += r;
		handle->_rrnum = rrnum;
	}
	r = ns_name_uncompress(handle->_msg, handle->_eom, handle->_msg_ptr, rr->name, NS_MAXDNAME);
	if (r < 0) return -1;
	handle->_msg_ptr += r;
	if (2 * NS_INT16SZ > handle->_eom - handle->_msg_ptr) goto size;
	NS_GET16(rr->type, handle->_msg_ptr);
	NS_GET16(rr->rr_class, handle->_msg_ptr);
	if (section != ns_s_qd) {
		if (NS_INT32SZ + NS_INT16SZ > handle->_eom - handle->_msg_ptr) goto size;
		NS_GET32(rr->ttl, handle->_msg_ptr);
		NS_GET16(rr->rdlength, handle->_msg_ptr);
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
	const unsigned char *eom : itype(_Ptr<const unsigned char>),
	const unsigned char *src : bounds(src, eom),
	char *dst : count(dstsiz) itype(_Array_ptr<char>),
	size_t dstsiz)
{
	int r;
	r = dn_expand(msg, eom, src, dst, dstsiz);
	if (r < 0) errno = EMSGSIZE;
	return r;
}

