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

	for (i = 0; i < count; i += 8 * 64) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(buf_p + 0 * 64));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(buf_p + 1 * 64));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(buf_p + 2 * 64));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(buf_p + 3 * 64));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(buf_p + 4 * 64));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(buf_p + 5 * 64));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(buf_p + 6 * 64));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(buf_p + 7 * 64));

		buf_p += 8 * 64;
	}

	x0 = _mm512_add_epi32(x0, x1);
	x2 = _mm512_add_epi32(x2, x3);
	x4 = _mm512_add_epi32(x4, x5);
	x6 = _mm512_add_epi32(x6, x7);

	x0 = _mm512_add_epi32(x0, x2);
	x4 = _mm512_add_epi32(x4, x6);

	x0 = _mm512_add_epi32(x0, x4);

	return _mm_extract_epi8(_mm512_extracti32x4_epi32(x0, 0), 0);
}

#endif /* __INTEL_COMPILER */
