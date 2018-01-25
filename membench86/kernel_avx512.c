#ifdef __INTEL_COMPILER

#include <immintrin.h>
#include "kernel.h"

unsigned read_memory_avx512(const void *buf, size_t count)
{
	const char *buf_p = buf;
	size_t i;

	__m512i x0 = _mm512_setzero_si512();
	__m512i x1 = x0;
	__m512i x2 = x0;
	__m512i x3 = x0;
	__m512i x4 = x0;
	__m512i x5 = x0;
	__m512i x6 = x0;
	__m512i x7 = x0;
	__m512i x8 = x0;
	__m512i x9 = x0;
	__m512i x10 = x0;
	__m512i x11 = x0;
	__m512i x12 = x0;
	__m512i x13 = x0;
	__m512i x14 = x0;
	__m512i x15 = x0;

	for (i = 0; i < count; i += 16 * 64) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(buf_p + 0 * 64));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(buf_p + 1 * 64));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(buf_p + 2 * 64));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(buf_p + 3 * 64));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(buf_p + 4 * 64));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(buf_p + 5 * 64));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(buf_p + 6 * 64));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(buf_p + 7 * 64));
		x8 = _mm512_xor_si512(x8, _mm512_load_si512(buf_p + 8 * 64));
		x9 = _mm512_xor_si512(x9, _mm512_load_si512(buf_p + 9 * 64));
		x10 = _mm512_xor_si512(x10, _mm512_load_si512(buf_p + 10 * 64));
		x11 = _mm512_xor_si512(x11, _mm512_load_si512(buf_p + 11 * 64));
		x12 = _mm512_xor_si512(x12, _mm512_load_si512(buf_p + 12 * 64));
		x13 = _mm512_xor_si512(x13, _mm512_load_si512(buf_p + 13 * 64));
		x14 = _mm512_xor_si512(x14, _mm512_load_si512(buf_p + 14 * 64));
		x15 = _mm512_xor_si512(x15, _mm512_load_si512(buf_p + 15 * 64));

		buf_p += 16 * 64;
	}

	x0 = _mm512_add_epi8(x0, x1);
	x2 = _mm512_add_epi8(x2, x3);
	x4 = _mm512_add_epi8(x4, x5);
	x6 = _mm512_add_epi8(x6, x7);
	x8 = _mm512_add_epi8(x8, x9);
	x10 = _mm512_add_epi8(x10, x11);
	x12 = _mm512_add_epi8(x12, x13);
	x14 = _mm512_add_epi8(x14, x15);

	x0 = _mm512_add_epi8(x0, x2);
	x4 = _mm512_add_epi8(x4, x6);
	x8 = _mm512_add_epi8(x8, x10);
	x12 = _mm512_add_epi8(x12, x14);

	x0 = _mm512_add_epi8(x0, x4);
	x8 = _mm512_add_epi8(x8, x12);

	x0 = _mm512_add_epi8(x0, x8);

	return _mm_extract_epi8(_mm512_extracti32x4_epi32(x0, 0), 0);
}

#endif /* __INTEL_COMPILER */
