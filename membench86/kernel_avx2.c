#include <immintrin.h>
#include "kernel.h"

unsigned read_memory_avx2(void *buf, size_t count)
{
	const char *buf_p = buf;
	size_t i;

	__m256i x0 = _mm256_setzero_si256();
	__m256i x1 = x0;
	__m256i x2 = x0;
	__m256i x3 = x0;
	__m256i x4 = x0;
	__m256i x5 = x0;
	__m256i x6 = x0;
	__m256i x7 = x0;

#define AT(x) (const __m256i *)(buf_p + (x))
	for (i = 0; i < count; i += 32 * 8) {
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

unsigned write_memory_avx2(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m256i x0 = _mm256_set1_epi8(0);
	__m256i x1 = _mm256_set1_epi8(1);
	__m256i x2 = _mm256_set1_epi8(2);
	__m256i x3 = _mm256_set1_epi8(3);
	__m256i x4 = _mm256_set1_epi8(4);
	__m256i x5 = _mm256_set1_epi8(5);
	__m256i x6 = _mm256_set1_epi8(6);
	__m256i x7 = _mm256_set1_epi8(7);

#define AT(x) (__m256i *)(buf_p + (x))
	for (i = 0; i < count; i += 32 * 8) {
		x0 = _mm256_add_epi8(x0, x7);
		x1 = _mm256_add_epi8(x1, x7);
		x2 = _mm256_add_epi8(x2, x7);
		x3 = _mm256_add_epi8(x3, x7);
		x4 = _mm256_add_epi8(x4, x7);
		x5 = _mm256_add_epi8(x5, x7);
		x6 = _mm256_add_epi8(x6, x7);
		x7 = _mm256_add_epi8(x7, x7);

		_mm256_store_si256(AT(32 * 0), x0);
		_mm256_store_si256(AT(32 * 1), x1);
		_mm256_store_si256(AT(32 * 2), x2);
		_mm256_store_si256(AT(32 * 3), x3);
		_mm256_store_si256(AT(32 * 4), x4);
		_mm256_store_si256(AT(32 * 5), x5);
		_mm256_store_si256(AT(32 * 6), x6);
		_mm256_store_si256(AT(32 * 7), x7);

		buf_p += 32 * 8;
	}
#undef AT
	return _mm256_extract_epi8(x0, 0);
}

unsigned write_memory_nt_avx2(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m256i x0 = _mm256_set1_epi8(0);
	__m256i x1 = _mm256_set1_epi8(1);
	__m256i x2 = _mm256_set1_epi8(2);
	__m256i x3 = _mm256_set1_epi8(3);
	__m256i x4 = _mm256_set1_epi8(4);
	__m256i x5 = _mm256_set1_epi8(5);
	__m256i x6 = _mm256_set1_epi8(6);
	__m256i x7 = _mm256_set1_epi8(7);

#define AT(x) (__m256i *)(buf_p + (x))
	for (i = 0; i < count; i += 32 * 8) {
		x0 = _mm256_add_epi8(x0, x7);
		x1 = _mm256_add_epi8(x1, x7);
		x2 = _mm256_add_epi8(x2, x7);
		x3 = _mm256_add_epi8(x3, x7);
		x4 = _mm256_add_epi8(x4, x7);
		x5 = _mm256_add_epi8(x5, x7);
		x6 = _mm256_add_epi8(x6, x7);
		x7 = _mm256_add_epi8(x7, x7);

		_mm256_stream_si256(AT(32 * 0), x0);
		_mm256_stream_si256(AT(32 * 1), x1);
		_mm256_stream_si256(AT(32 * 2), x2);
		_mm256_stream_si256(AT(32 * 3), x3);
		_mm256_stream_si256(AT(32 * 4), x4);
		_mm256_stream_si256(AT(32 * 5), x5);
		_mm256_stream_si256(AT(32 * 6), x6);
		_mm256_stream_si256(AT(32 * 7), x7);

		buf_p += 32 * 8;
	}
#undef AT
	return _mm256_extract_epi8(x0, 0);
}

unsigned copy_memory_avx2(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m256i x0 = _mm256_setzero_si256();
	__m256i x1 = x0;
	__m256i x2 = x0;
	__m256i x3 = x0;
	__m256i x4 = x0;
	__m256i x5 = x0;
	__m256i x6 = x0;
	__m256i x7 = x0;

#define ATS(x) (const __m256i *)(src_p + (x))
#define ATD(x) (__m256i *)(dst_p + (x))
	for (i = 0; i < count / 2; i += 32 * 8) {
		x0 = _mm256_xor_si256(x0, _mm256_load_si256(ATS(32 * 0)));
		x1 = _mm256_xor_si256(x1, _mm256_load_si256(ATS(32 * 1)));
		x2 = _mm256_xor_si256(x2, _mm256_load_si256(ATS(32 * 2)));
		x3 = _mm256_xor_si256(x3, _mm256_load_si256(ATS(32 * 3)));
		x4 = _mm256_xor_si256(x4, _mm256_load_si256(ATS(32 * 4)));
		x5 = _mm256_xor_si256(x5, _mm256_load_si256(ATS(32 * 5)));
		x6 = _mm256_xor_si256(x6, _mm256_load_si256(ATS(32 * 6)));
		x7 = _mm256_xor_si256(x7, _mm256_load_si256(ATS(32 * 7)));

		_mm256_store_si256(ATD(32 * 0), x0);
		_mm256_store_si256(ATD(32 * 1), x1);
		_mm256_store_si256(ATD(32 * 2), x2);
		_mm256_store_si256(ATD(32 * 3), x3);
		_mm256_store_si256(ATD(32 * 4), x4);
		_mm256_store_si256(ATD(32 * 5), x5);
		_mm256_store_si256(ATD(32 * 6), x6);
		_mm256_store_si256(ATD(32 * 7), x7);

		src_p += 32 * 8;
		dst_p += 32 * 8;
	}
#undef ATD
#undef ATS

	x0 = _mm256_add_epi8(x0, x1);
	x2 = _mm256_add_epi8(x2, x3);
	x4 = _mm256_add_epi8(x4, x5);
	x6 = _mm256_add_epi8(x6, x7);

	x0 = _mm256_add_epi8(x0, x2);
	x4 = _mm256_add_epi8(x4, x6);

	x0 = _mm256_add_epi8(x0, x4);

	return _mm256_extract_epi8(x0, 0);
}

unsigned copy_memory_nt_avx2(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m256i x0 = _mm256_setzero_si256();
	__m256i x1 = x0;
	__m256i x2 = x0;
	__m256i x3 = x0;
	__m256i x4 = x0;
	__m256i x5 = x0;
	__m256i x6 = x0;
	__m256i x7 = x0;

#define ATS(x) (const __m256i *)(src_p + (x))
#define ATD(x) (__m256i *)(dst_p + (x))
	for (i = 0; i < count / 2; i += 32 * 8) {
		x0 = _mm256_xor_si256(x0, _mm256_load_si256(ATS(32 * 0)));
		x1 = _mm256_xor_si256(x1, _mm256_load_si256(ATS(32 * 1)));
		x2 = _mm256_xor_si256(x2, _mm256_load_si256(ATS(32 * 2)));
		x3 = _mm256_xor_si256(x3, _mm256_load_si256(ATS(32 * 3)));
		x4 = _mm256_xor_si256(x4, _mm256_load_si256(ATS(32 * 4)));
		x5 = _mm256_xor_si256(x5, _mm256_load_si256(ATS(32 * 5)));
		x6 = _mm256_xor_si256(x6, _mm256_load_si256(ATS(32 * 6)));
		x7 = _mm256_xor_si256(x7, _mm256_load_si256(ATS(32 * 7)));

		_mm256_stream_si256(ATD(32 * 0), x0);
		_mm256_stream_si256(ATD(32 * 1), x1);
		_mm256_stream_si256(ATD(32 * 2), x2);
		_mm256_stream_si256(ATD(32 * 3), x3);
		_mm256_stream_si256(ATD(32 * 4), x4);
		_mm256_stream_si256(ATD(32 * 5), x5);
		_mm256_stream_si256(ATD(32 * 6), x6);
		_mm256_stream_si256(ATD(32 * 7), x7);

		src_p += 32 * 8;
		dst_p += 32 * 8;
	}
#undef ATD
#undef ATS

	x0 = _mm256_add_epi8(x0, x1);
	x2 = _mm256_add_epi8(x2, x3);
	x4 = _mm256_add_epi8(x4, x5);
	x6 = _mm256_add_epi8(x6, x7);

	x0 = _mm256_add_epi8(x0, x2);
	x4 = _mm256_add_epi8(x4, x6);

	x0 = _mm256_add_epi8(x0, x4);

	return _mm256_extract_epi8(x0, 0);
}
