#include <immintrin.h>
#include "kernel.h"

unsigned read_memory_avx2(const void *buf, size_t count)
{
	const char *buf_p = (const char *)buf;

	__m256i x0 = _mm256_setzero_si256();
	__m256i x1 = x0;
	__m256i x2 = x0;
	__m256i x3 = x0;
	__m256i x4 = x0;
	__m256i x5 = x0;
	__m256i x6 = x0;
	__m256i x7 = x0;

#define AT(x) (const __m256i *)((const char *)buf_p + (x))
	for (size_t i = 0; i < count; i += 32 * 8) {
		x0 = _mm256_xor_si256(x0, _mm256_load_si256(AT(32 * 0)));
		x1 = _mm256_xor_si256(x1, _mm256_load_si256(AT(32 * 1)));
		x2 = _mm256_xor_si256(x2, _mm256_load_si256(AT(32 * 2)));
		x3 = _mm256_xor_si256(x3, _mm256_load_si256(AT(32 * 3)));
		x4 = _mm256_xor_si256(x4, _mm256_load_si256(AT(32 * 4)));
		x5 = _mm256_xor_si256(x5, _mm256_load_si256(AT(32 * 5)));
		x6 = _mm256_xor_si256(x6, _mm256_load_si256(AT(32 * 6)));
		x7 = _mm256_xor_si256(x7, _mm256_load_si256(AT(32 * 7)));

		buf_p += 32 * 8;
	}
#undef AT

	x0 = _mm256_add_epi8(x0, x1);
	x2 = _mm256_add_epi8(x2, x3);
	x4 = _mm256_add_epi8(x4, x5);
	x6 = _mm256_add_epi8(x6, x7);

	x0 = _mm256_add_epi8(x0, x2);
	x4 = _mm256_add_epi8(x4, x6);

	x0 = _mm256_add_epi8(x0, x4);

	return _mm256_extract_epi8(x0, 0);
}
