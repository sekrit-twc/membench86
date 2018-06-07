#include <emmintrin.h>
#include "kernel.h"

unsigned read_memory_sse2(void *buf, size_t count)
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

#define AT(x) (const __m128i *)(buf_p + (x))
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

unsigned write_memory_sse2(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m128i x0 = _mm_set1_epi8(0);
	__m128i x1 = _mm_set1_epi8(1);
	__m128i x2 = _mm_set1_epi8(2);
	__m128i x3 = _mm_set1_epi8(3);
	__m128i x4 = _mm_set1_epi8(4);
	__m128i x5 = _mm_set1_epi8(5);
	__m128i x6 = _mm_set1_epi8(6);
	__m128i x7 = _mm_set1_epi8(7);

#define AT(x) (__m128i *)(buf_p + (x))
	for (i = 0; i < count; i += 16 * 8) {
		x0 = _mm_add_epi8(x0, x7);
		x1 = _mm_add_epi8(x1, x7);
		x2 = _mm_add_epi8(x2, x7);
		x3 = _mm_add_epi8(x3, x7);
		x4 = _mm_add_epi8(x4, x7);
		x5 = _mm_add_epi8(x5, x7);
		x6 = _mm_add_epi8(x6, x7);
		x7 = _mm_add_epi8(x7, x7);

		_mm_store_si128(AT(16 * 0), x0);
		_mm_store_si128(AT(16 * 1), x1);
		_mm_store_si128(AT(16 * 2), x2);
		_mm_store_si128(AT(16 * 3), x3);
		_mm_store_si128(AT(16 * 4), x4);
		_mm_store_si128(AT(16 * 5), x5);
		_mm_store_si128(AT(16 * 6), x6);
		_mm_store_si128(AT(16 * 7), x7);

		buf_p += 16 * 8;
	}
#undef AT
	return _mm_cvtsi128_si32(x0) & 0xFF;
}

unsigned write_memory_nt_sse2(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m128i x0 = _mm_set1_epi8(0);
	__m128i x1 = _mm_set1_epi8(1);
	__m128i x2 = _mm_set1_epi8(2);
	__m128i x3 = _mm_set1_epi8(3);
	__m128i x4 = _mm_set1_epi8(4);
	__m128i x5 = _mm_set1_epi8(5);
	__m128i x6 = _mm_set1_epi8(6);
	__m128i x7 = _mm_set1_epi8(7);

#define AT(x) (__m128i *)(buf_p + (x))
	for (i = 0; i < count; i += 16 * 8) {
		x0 = _mm_add_epi8(x0, x7);
		x1 = _mm_add_epi8(x1, x7);
		x2 = _mm_add_epi8(x2, x7);
		x3 = _mm_add_epi8(x3, x7);
		x4 = _mm_add_epi8(x4, x7);
		x5 = _mm_add_epi8(x5, x7);
		x6 = _mm_add_epi8(x6, x7);
		x7 = _mm_add_epi8(x7, x7);

		_mm_stream_si128(AT(16 * 0), x0);
		_mm_stream_si128(AT(16 * 1), x1);
		_mm_stream_si128(AT(16 * 2), x2);
		_mm_stream_si128(AT(16 * 3), x3);
		_mm_stream_si128(AT(16 * 4), x4);
		_mm_stream_si128(AT(16 * 5), x5);
		_mm_stream_si128(AT(16 * 6), x6);
		_mm_stream_si128(AT(16 * 7), x7);

		buf_p += 16 * 8;
	}
#undef AT
	return _mm_cvtsi128_si32(x0) & 0xFF;
}

unsigned copy_memory_sse2(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m128i x0 = _mm_setzero_si128();
	__m128i x1 = x0;
	__m128i x2 = x0;
	__m128i x3 = x0;
	__m128i x4 = x0;
	__m128i x5 = x0;
	__m128i x6 = x0;
	__m128i x7 = x0;

#define ATS(x) (const __m128i *)(src_p + (x))
#define ATD(x) (__m128i *)(dst_p + (x))
	for (i = 0; i < count / 2; i += 16 * 8) {
		x0 = _mm_xor_si128(x0, _mm_load_si128(ATS(16 * 0)));
		x1 = _mm_xor_si128(x1, _mm_load_si128(ATS(16 * 1)));
		x2 = _mm_xor_si128(x2, _mm_load_si128(ATS(16 * 2)));
		x3 = _mm_xor_si128(x3, _mm_load_si128(ATS(16 * 3)));
		x4 = _mm_xor_si128(x4, _mm_load_si128(ATS(16 * 4)));
		x5 = _mm_xor_si128(x5, _mm_load_si128(ATS(16 * 5)));
		x6 = _mm_xor_si128(x6, _mm_load_si128(ATS(16 * 6)));
		x7 = _mm_xor_si128(x7, _mm_load_si128(ATS(16 * 7)));

		_mm_store_si128(ATD(16 * 0), x0);
		_mm_store_si128(ATD(16 * 1), x1);
		_mm_store_si128(ATD(16 * 2), x2);
		_mm_store_si128(ATD(16 * 3), x3);
		_mm_store_si128(ATD(16 * 4), x4);
		_mm_store_si128(ATD(16 * 5), x5);
		_mm_store_si128(ATD(16 * 6), x6);
		_mm_store_si128(ATD(16 * 7), x7);

		src_p += 16 * 8;
		dst_p += 16 * 8;
	}
#undef ATD
#undef ATS

	x0 = _mm_add_epi8(x0, x1);
	x2 = _mm_add_epi8(x2, x3);
	x4 = _mm_add_epi8(x4, x5);
	x6 = _mm_add_epi8(x6, x7);

	x0 = _mm_add_epi8(x0, x2);
	x4 = _mm_add_epi8(x4, x6);

	x0 = _mm_add_epi8(x0, x4);

	return _mm_cvtsi128_si32(x0) & 0xFF;
}

unsigned copy_memory_nt_sse2(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m128i x0 = _mm_setzero_si128();
	__m128i x1 = x0;
	__m128i x2 = x0;
	__m128i x3 = x0;
	__m128i x4 = x0;
	__m128i x5 = x0;
	__m128i x6 = x0;
	__m128i x7 = x0;

#define ATS(x) (const __m128i *)(src_p + (x))
#define ATD(x) (__m128i *)(dst_p + (x))
	for (i = 0; i < count / 2; i += 16 * 8) {
		x0 = _mm_xor_si128(x0, _mm_load_si128(ATS(16 * 0)));
		x1 = _mm_xor_si128(x1, _mm_load_si128(ATS(16 * 1)));
		x2 = _mm_xor_si128(x2, _mm_load_si128(ATS(16 * 2)));
		x3 = _mm_xor_si128(x3, _mm_load_si128(ATS(16 * 3)));
		x4 = _mm_xor_si128(x4, _mm_load_si128(ATS(16 * 4)));
		x5 = _mm_xor_si128(x5, _mm_load_si128(ATS(16 * 5)));
		x6 = _mm_xor_si128(x6, _mm_load_si128(ATS(16 * 6)));
		x7 = _mm_xor_si128(x7, _mm_load_si128(ATS(16 * 7)));

		_mm_stream_si128(ATD(16 * 0), x0);
		_mm_stream_si128(ATD(16 * 1), x1);
		_mm_stream_si128(ATD(16 * 2), x2);
		_mm_stream_si128(ATD(16 * 3), x3);
		_mm_stream_si128(ATD(16 * 4), x4);
		_mm_stream_si128(ATD(16 * 5), x5);
		_mm_stream_si128(ATD(16 * 6), x6);
		_mm_stream_si128(ATD(16 * 7), x7);

		src_p += 16 * 8;
		dst_p += 16 * 8;
	}
#undef ATD
#undef ATS

	x0 = _mm_add_epi8(x0, x1);
	x2 = _mm_add_epi8(x2, x3);
	x4 = _mm_add_epi8(x4, x5);
	x6 = _mm_add_epi8(x6, x7);

	x0 = _mm_add_epi8(x0, x2);
	x4 = _mm_add_epi8(x4, x6);

	x0 = _mm_add_epi8(x0, x4);

	return _mm_cvtsi128_si32(x0) & 0xFF;
}
