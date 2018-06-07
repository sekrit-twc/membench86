#include <limits.h>
#include <stdint.h>
#include "kernel.h"

unsigned read_memory_c(void *buf, size_t count)
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

unsigned write_memory_c(void *buf, size_t count)
{
#if UINTPTR_MAX == ULLONG_MAX
	unsigned long long *buf_p = buf;
	size_t i;

	unsigned long long val = 0;

	for (i = 0; i < count; i += sizeof(unsigned long long) * 8) {
		val = i;

		buf_p[i / sizeof(unsigned long long) + 0] = val + 0;
		buf_p[i / sizeof(unsigned long long) + 1] = val + 1;
		buf_p[i / sizeof(unsigned long long) + 2] = val + 2;
		buf_p[i / sizeof(unsigned long long) + 3] = val + 3;
		buf_p[i / sizeof(unsigned long long) + 4] = val + 4;
		buf_p[i / sizeof(unsigned long long) + 5] = val + 5;
		buf_p[i / sizeof(unsigned long long) + 6] = val + 6;
		buf_p[i / sizeof(unsigned long long) + 7] = val + 7;
	}
#else
	unsigned *buf_p = buf;
	size_t i;

	unsigned val = 0;

	for (i = 0; i < count; i += sizeof(unsigned) * 4) {
		buf_p[i / sizeof(unsigned) + 0] = val + 0;
		buf_p[i / sizeof(unsigned) + 1] = val + 1;
		buf_p[i / sizeof(unsigned) + 2] = val + 2;
		buf_p[i / sizeof(unsigned) + 3] = val + 3;
	}
#endif /* INTPTR_MAX == INT_LEAST64_MAX */
	return (unsigned)val;
}

unsigned copy_memory_c(void *buf, size_t count)
{
#if UINTPTR_MAX == ULLONG_MAX
	const unsigned long long *src_p = buf;
	unsigned long long *dst_p = (unsigned long long *)((char *)buf + count / 2);
	size_t i;

	unsigned long long x0 = 0;
	unsigned long long x1 = 0;
	unsigned long long x2 = 0;
	unsigned long long x3 = 0;
	unsigned long long x4 = 0;
	unsigned long long x5 = 0;
	unsigned long long x6 = 0;
	unsigned long long x7 = 0;

	for (i = 0; i < count / 2; i += sizeof(unsigned long long) * 8) {
		x0 += src_p[i / sizeof(unsigned long long) + 0];
		x1 += src_p[i / sizeof(unsigned long long) + 1];
		x2 += src_p[i / sizeof(unsigned long long) + 2];
		x3 += src_p[i / sizeof(unsigned long long) + 3];
		x4 += src_p[i / sizeof(unsigned long long) + 4];
		x5 += src_p[i / sizeof(unsigned long long) + 5];
		x6 += src_p[i / sizeof(unsigned long long) + 6];
		x7 += src_p[i / sizeof(unsigned long long) + 7];

		dst_p[i / sizeof(unsigned long long) + 0] = x0;
		dst_p[i / sizeof(unsigned long long) + 1] = x1;
		dst_p[i / sizeof(unsigned long long) + 2] = x2;
		dst_p[i / sizeof(unsigned long long) + 3] = x3;
		dst_p[i / sizeof(unsigned long long) + 4] = x4;
		dst_p[i / sizeof(unsigned long long) + 5] = x5;
		dst_p[i / sizeof(unsigned long long) + 6] = x6;
		dst_p[i / sizeof(unsigned long long) + 7] = x7;
	}

	x0 = ((x0 + x1) + (x2 + x3)) + ((x4 + x5) + (x6 + x7));
#else
	const unsigned *src_p = buf;
	unsigned *dst_p = (unsigned *)((char *)buf + count / 2);
	size_t i;

	unsigned x0 = 0;
	unsigned x1 = 0;
	unsigned x2 = 0;
	unsigned x3 = 0;

	for (i = 0; i < count / 2; i += sizeof(unsigned) * 4) {
		x0 += src_p[i / sizeof(unsigned) + 0];
		x1 += src_p[i / sizeof(unsigned) + 1];
		x2 += src_p[i / sizeof(unsigned) + 2];
		x3 += src_p[i / sizeof(unsigned) + 3];

		dst_p[i / sizeof(unsigned) + 0] = x0;
		dst_p[i / sizeof(unsigned) + 1] = x1;
		dst_p[i / sizeof(unsigned) + 2] = x2;
		dst_p[i / sizeof(unsigned) + 3] = x3;
	}

	x0 = (x0 + x1) + (x2 + x3);
#endif /* INTPTR_MAX == INT_LEAST64_MAX */
	return (unsigned)x0;
}
