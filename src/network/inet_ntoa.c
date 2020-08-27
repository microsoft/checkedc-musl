#include <arpa/inet.h>
#include <stdio.h>
#pragma CHECKED_SCOPE on

// inet_ntoa allocates and returns a null-terminated buffer of 16 bytes, which
// corresponds to an nt_array_ptr of count(15) (16 minus 1 for the NULL byte).
char *inet_ntoa(struct in_addr in) : count(15) itype(_Nt_array_ptr<char>)
{
	static char buf _Nt_checked[16];
	// Struct in_addr has 32 bytes. Cast it to unsigned char pointer with count(4).
	_Array_ptr<unsigned char> a : count(4) = (_Array_ptr<unsigned char>)&in;
	_Unchecked {
		snprintf((char *)buf, sizeof buf, "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
	}
	return buf;
}
