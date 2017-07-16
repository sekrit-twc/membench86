#include <immintrin.h>
#include "kernel.h"

unsigned read_memory_avx512(const void *buf, size_t count)
{
	const char *buf_p = (const char *)buf;

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

#define AT(x) (const __m512i *)((const char *)buf_p + (x))
	for (size_t i = 0; i < count; i += 64 * 16) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(AT(64 * 0)));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(AT(64 * 1)));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(AT(64 * 2)));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(AT(64 * 3)));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(AT(64 * 4)));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(AT(64 * 5)));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(AT(64 * 6)));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(AT(64 * 7)));
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(AT(64 * 8)));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(AT(64 * 9)));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(AT(64 * 10)));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(AT(64 * 11)));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(AT(64 * 12)));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(AT(64 * 13)));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(AT(64 * 14)));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(AT(64 * 15)));

		buf_p += 64 * 16;
	}
#undef AT

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
