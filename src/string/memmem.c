#define _GNU_SOURCE
#include <string.h>
#include <stdint.h>

static _Nt_array_ptr<char> twobyte_memmem(_Nt_array_ptr<const unsigned char> h : count(k), size_t k, _Array_ptr<const unsigned char> n : count(2)) : count(k)
{
	uint16_t nw = n[0]<<8 | n[1], hw = h[0]<<8 | h[1];
	for (h+=2, k-=2; k; k--, hw = hw<<8 | *h++)
		if (hw == nw) return (_Nt_array_ptr<char>)h-2;
	return hw == nw ? (_Nt_array_ptr<char>)h-2 : 0;
}

static _Nt_array_ptr<char> threebyte_memmem(_Nt_array_ptr<const unsigned char> h : count(k), size_t k, _Nt_array_ptr<const unsigned char> n : count(3)) : count(k)
{
	uint32_t nw = (uint32_t)n[0]<<24 | n[1]<<16 | n[2]<<8;
	uint32_t hw = (uint32_t)h[0]<<24 | h[1]<<16 | h[2]<<8;
	for (h+=3, k-=3; k; k--, hw = (hw|*h++)<<8)
		if (hw == nw) return (_Nt_array_ptr<char>)h-3;
	return hw == nw ? (_Nt_array_ptr<char>)h-3 : 0;
}

static _Nt_array_ptr<char> fourbyte_memmem(_Nt_array_ptr<const unsigned char> h : count(k), size_t k, _Nt_array_ptr<const unsigned char> n: count(4)) : count(k)
{
	uint32_t nw = (uint32_t)n[0]<<24 | n[1]<<16 | n[2]<<8 | n[3];
	uint32_t hw = (uint32_t)h[0]<<24 | h[1]<<16 | h[2]<<8 | h[3];
	for (h+=4, k-=4; k; k--, hw = hw<<8 | *h++)
		if (hw == nw) return (_Nt_array_ptr<char>)h-4;
	return hw == nw ? (_Nt_array_ptr<char>)h-4 : 0;
}

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

static _Nt_array_ptr<char> twoway_memmem(_Nt_array_ptr<const unsigned char> h, _Nt_array_ptr<const unsigned char> z, _Nt_array_ptr<const unsigned char> n : count(l), size_t l)
{
	size_t i, ip, jp, k, p, ms, p0, mem, mem0;
	size_t byteset _Checked[32 / sizeof(size_t)] = { 0 };
	size_t shift _Checked[256];

	/* Computing length of needle and fill shift table */
	for (i=0; i<l; i++)
		BITOP(byteset, n[i], |=), shift[n[i]] = i+1;

	/* Compute maximal suffix */
	ip = -1; jp = 0; k = p = 1;
	while (jp+k<l) {
		if (n[ip+k] == n[jp+k]) {
			if (k == p) {
				jp += p;
				k = 1;
			} else k++;
		} else if (n[ip+k] > n[jp+k]) {
			jp += k;
			k = 1;
			p = jp - ip;
		} else {
			ip = jp++;
			k = p = 1;
		}
	}
	ms = ip;
	p0 = p;

	/* And with the opposite comparison */
	ip = -1; jp = 0; k = p = 1;
	while (jp+k<l) {
		if (n[ip+k] == n[jp+k]) {
			if (k == p) {
				jp += p;
				k = 1;
			} else k++;
		} else if (n[ip+k] < n[jp+k]) {
			jp += k;
			k = 1;
			p = jp - ip;
		} else {
			ip = jp++;
			k = p = 1;
		}
	}
	if (ip+1 > ms+1) ms = ip;
	else p = p0;

	/* Periodic needle? */
	if (memcmp(n, n+p, ms+1)) {
		mem0 = 0;
		p = MAX(ms, l-ms-1) + 1;
	} else mem0 = l-p;
	mem = 0;

	/* Search loop */
	for (;;) {
		/* If remainder of haystack is shorter than needle, done */
		if (z-h < l) return 0;

		/* Check last byte first; advance by shift on mismatch */
		if (BITOP(byteset, h[l-1], &)) {
			k = l-shift[h[l-1]];
			if (k) {
				if (k < mem) k = mem;
				h += k;
				mem = 0;
				continue;
			}
		} else {
			h += l;
			mem = 0;
			continue;
		}

		/* Compare right half */
		for (k=MAX(ms+1,mem); k<l && n[k] == h[k]; k++);
		if (k < l) {
			h += k-ms;
			mem = 0;
			continue;
		}
		/* Compare left half */
		for (k=ms+1; k>mem && n[k-1] == h[k-1]; k--);
		if (k <= mem) return (_Nt_array_ptr<char>)h;
		h += p;
		mem = mem0;
	}
}

void *memmem(const void *h0 : itype(_Array_ptr<const void>) byte_count(k), size_t k, const void *n0 : itype(_Array_ptr<const void>) byte_count(l), size_t l) : itype(_Array_ptr<void>) byte_count(k)
{
	  _Nt_array_ptr<const unsigned char> h : count(k) = _Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(h0, count(k));   
	  _Nt_array_ptr<const unsigned char> n : count(l) = _Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(n0, count(l));   

	/* Return immediately on empty needle */
	if (!l) return (void *)h;

	/* Return immediately when needle is longer than haystack */
	if (k<l) return 0;

	/* Use faster algorithms for short needles */
	h = _Dynamic_bounds_cast<_Nt_array_ptr<const unsigned char>>(memchr(h0, *n, k), count(k));
	if (!h || l==1) return (void *)h;
	k -= h - (const unsigned char *)h0;
	if (k<l) return 0;
	if (l==2) return twobyte_memmem(h, k, n);
	if (l==3) return threebyte_memmem(h, k, n);
	if (l==4) return fourbyte_memmem(h, k, n);

	return twoway_memmem(h, h+k, n, l);
}
