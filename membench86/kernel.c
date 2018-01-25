#include <limits.h>
#include <stdint.h>
#include "kernel.h"

unsigned read_memory_c(const void *buf, size_t count)
{
#if UINTPTR_MAX == ULLONG_MAX
	const unsigned long long *buf_p = buf;
	size_t i;

	unsigned long long x0 = 0;
	unsigned long long x1 = 0;
	unsigned long long x2 = 0;
	unsigned long long x3 = 0;
	unsigned long long x4 = 0;
	unsigned long long x5 = 0;
	unsigned long long x6 = 0;
	unsigned long long x7 = 0;

	for (i = 0; i < count; i += sizeof(unsigned long long) * 8) {
		x0 += buf_p[i / sizeof(unsigned long long) + 0];
		x1 += buf_p[i / sizeof(unsigned long long) + 1];
		x2 += buf_p[i / sizeof(unsigned long long) + 2];
		x3 += buf_p[i / sizeof(unsigned long long) + 3];
		x4 += buf_p[i / sizeof(unsigned long long) + 4];
		x5 += buf_p[i / sizeof(unsigned long long) + 5];
		x6 += buf_p[i / sizeof(unsigned long long) + 6];
		x7 += buf_p[i / sizeof(unsigned long long) + 7];
	}

	x0 = ((x0 + x1) + (x2 + x3)) + ((x4 + x5) + (x6 + x7));
#else
	const unsigned *buf_p = buf;
	size_t i;

	unsigned x0 = 0;
	unsigned x1 = 0;
	unsigned x2 = 0;
	unsigned x3 = 0;

	for (i = 0; i < count; i += sizeof(unsigned) * 4) {
		x0 += buf_p[i / sizeof(unsigned) + 0];
		x1 += buf_p[i / sizeof(unsigned) + 1];
		x2 += buf_p[i / sizeof(unsigned) + 2];
		x3 += buf_p[i / sizeof(unsigned) + 3];
	}

	x0 = (x0 + x1) + (x2 + x3);
#endif /* INTPTR_MAX == INT_LEAST64_MAX */
	return (unsigned)x0;
}
