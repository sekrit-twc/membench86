#include <emmintrin.h>
#include "kernel.h"

unsigned read_memory_sse2(const void *buf, size_t count)
{
	const char *buf_p = buf;
	size_t i;

	__m128i x0 = _mm_setzero_si128();
	__m128i x1 = x0;
	__m128i x2 = x0;
	__m128i x3 = x0;
	__m128i x4 = x0;
	__m128i x5 = x0;
	__m128i x6 = x0;
	__m128i x7 = x0;

#define AT(x) (const __m128i *)((const char *)buf_p + (x))
	for (i = 0; i < count; i += 16 * 8) {
		x0 = _mm_xor_si128(x0, _mm_load_si128(AT(16 * 0)));
		x1 = _mm_xor_si128(x1, _mm_load_si128(AT(16 * 1)));
		x2 = _mm_xor_si128(x2, _mm_load_si128(AT(16 * 2)));
		x3 = _mm_xor_si128(x3, _mm_load_si128(AT(16 * 3)));
		x4 = _mm_xor_si128(x4, _mm_load_si128(AT(16 * 4)));
		x5 = _mm_xor_si128(x5, _mm_load_si128(AT(16 * 5)));
		x6 = _mm_xor_si128(x6, _mm_load_si128(AT(16 * 6)));
		x7 = _mm_xor_si128(x7, _mm_load_si128(AT(16 * 7)));

		buf_p += 16 * 8;
	}
#undef AT

	x0 = _mm_add_epi8(x0, x1);
	x2 = _mm_add_epi8(x2, x3);
	x4 = _mm_add_epi8(x4, x5);
	x6 = _mm_add_epi8(x6, x7);

	x0 = _mm_add_epi8(x0, x2);
	x4 = _mm_add_epi8(x4, x6);

	x0 = _mm_add_epi8(x0, x4);

	return _mm_cvtsi128_si32(x0) & 0xFF;
}
