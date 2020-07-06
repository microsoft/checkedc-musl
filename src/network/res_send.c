#include <resolv.h>

int __res_send(const unsigned char *msg : count(msglen),
	int msglen,
	unsigned char *answer : count(anslen),
	int anslen)
{
	int r = __res_msend(1, &msg, &msglen, &answer, &anslen, anslen);
	return r<0 || !anslen ? -1 : anslen;
}

weak_alias(__res_send, res_send);
