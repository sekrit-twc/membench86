#include <immintrin.h>
#include "kernel.h"

unsigned read_memory_avx512(void *buf, size_t count)
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

	for (i = 0; i < count; i += 64 * 8) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(buf_p + 0 * 64));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(buf_p + 1 * 64));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(buf_p + 2 * 64));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(buf_p + 3 * 64));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(buf_p + 4 * 64));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(buf_p + 5 * 64));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(buf_p + 6 * 64));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(buf_p + 7 * 64));

		buf_p += 64 * 8;
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

unsigned write_memory_avx512(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m512i x0 = _mm512_set1_epi8(0);
	__m512i x1 = _mm512_set1_epi8(1);
	__m512i x2 = _mm512_set1_epi8(2);
	__m512i x3 = _mm512_set1_epi8(3);
	__m512i x4 = _mm512_set1_epi8(4);
	__m512i x5 = _mm512_set1_epi8(5);
	__m512i x6 = _mm512_set1_epi8(6);
	__m512i x7 = _mm512_set1_epi8(7);

	for (i = 0; i < count; i += 64 * 8) {
		x0 = _mm512_add_epi8(x0, x7);
		x1 = _mm512_add_epi8(x1, x7);
		x2 = _mm512_add_epi8(x2, x7);
		x3 = _mm512_add_epi8(x3, x7);
		x4 = _mm512_add_epi8(x4, x7);
		x5 = _mm512_add_epi8(x5, x7);
		x6 = _mm512_add_epi8(x6, x7);
		x7 = _mm512_add_epi8(x7, x7);

		_mm512_store_si512(buf_p + 64 * 0, x0);
		_mm512_store_si512(buf_p + 64 * 1, x1);
		_mm512_store_si512(buf_p + 64 * 2, x2);
		_mm512_store_si512(buf_p + 64 * 3, x3);
		_mm512_store_si512(buf_p + 64 * 4, x4);
		_mm512_store_si512(buf_p + 64 * 5, x5);
		_mm512_store_si512(buf_p + 64 * 6, x6);
		_mm512_store_si512(buf_p + 64 * 7, x7);

		buf_p += 64 * 8;
	}

	return _mm_extract_epi8(_mm512_extracti32x4_epi32(x0, 0), 0);
}

unsigned write_memory_nt_avx512(void *buf, size_t count)
{
	char *buf_p = buf;
	size_t i;

	__m512i x0 = _mm512_set1_epi8(0);
	__m512i x1 = _mm512_set1_epi8(1);
	__m512i x2 = _mm512_set1_epi8(2);
	__m512i x3 = _mm512_set1_epi8(3);
	__m512i x4 = _mm512_set1_epi8(4);
	__m512i x5 = _mm512_set1_epi8(5);
	__m512i x6 = _mm512_set1_epi8(6);
	__m512i x7 = _mm512_set1_epi8(7);

	for (i = 0; i < count; i += 64 * 8) {
		x0 = _mm512_add_epi8(x0, x7);
		x1 = _mm512_add_epi8(x1, x7);
		x2 = _mm512_add_epi8(x2, x7);
		x3 = _mm512_add_epi8(x3, x7);
		x4 = _mm512_add_epi8(x4, x7);
		x5 = _mm512_add_epi8(x5, x7);
		x6 = _mm512_add_epi8(x6, x7);
		x7 = _mm512_add_epi8(x7, x7);

		_mm512_stream_si512(buf_p + 64 * 0, x0);
		_mm512_stream_si512(buf_p + 64 * 1, x1);
		_mm512_stream_si512(buf_p + 64 * 2, x2);
		_mm512_stream_si512(buf_p + 64 * 3, x3);
		_mm512_stream_si512(buf_p + 64 * 4, x4);
		_mm512_stream_si512(buf_p + 64 * 5, x5);
		_mm512_stream_si512(buf_p + 64 * 6, x6);
		_mm512_stream_si512(buf_p + 64 * 7, x7);

		buf_p += 64 * 8;
	}

	return _mm_extract_epi8(_mm512_extracti32x4_epi32(x0, 0), 0);
}

unsigned copy_memory_avx512(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m512i x0 = _mm512_setzero_si512();
	__m512i x1 = x0;
	__m512i x2 = x0;
	__m512i x3 = x0;
	__m512i x4 = x0;
	__m512i x5 = x0;
	__m512i x6 = x0;
	__m512i x7 = x0;

	for (i = 0; i < count / 2; i += 64 * 8) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(src_p + 0 * 64));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(src_p + 1 * 64));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(src_p + 2 * 64));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(src_p + 3 * 64));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(src_p + 4 * 64));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(src_p + 5 * 64));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(src_p + 6 * 64));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(src_p + 7 * 64));

		_mm512_store_si512(dst_p + 0 * 64, x0);
		_mm512_store_si512(dst_p + 1 * 64, x1);
		_mm512_store_si512(dst_p + 2 * 64, x2);
		_mm512_store_si512(dst_p + 3 * 64, x3);
		_mm512_store_si512(dst_p + 4 * 64, x4);
		_mm512_store_si512(dst_p + 5 * 64, x5);
		_mm512_store_si512(dst_p + 6 * 64, x6);
		_mm512_store_si512(dst_p + 7 * 64, x7);

		src_p += 64 * 8;
		dst_p += 64 * 8;
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

unsigned copy_memory_nt_avx512(void *buf, size_t count)
{
	const char *src_p = buf;
	char *dst_p = (char *)buf + count / 2;
	size_t i;

	__m512i x0 = _mm512_setzero_si512();
	__m512i x1 = x0;
	__m512i x2 = x0;
	__m512i x3 = x0;
	__m512i x4 = x0;
	__m512i x5 = x0;
	__m512i x6 = x0;
	__m512i x7 = x0;

	for (i = 0; i < count / 2; i += 64 * 8) {
		x0 = _mm512_xor_si512(x0, _mm512_load_si512(src_p + 0 * 64));
		x1 = _mm512_xor_si512(x1, _mm512_load_si512(src_p + 1 * 64));
		x2 = _mm512_xor_si512(x2, _mm512_load_si512(src_p + 2 * 64));
		x3 = _mm512_xor_si512(x3, _mm512_load_si512(src_p + 3 * 64));
		x4 = _mm512_xor_si512(x4, _mm512_load_si512(src_p + 4 * 64));
		x5 = _mm512_xor_si512(x5, _mm512_load_si512(src_p + 5 * 64));
		x6 = _mm512_xor_si512(x6, _mm512_load_si512(src_p + 6 * 64));
		x7 = _mm512_xor_si512(x7, _mm512_load_si512(src_p + 7 * 64));

		_mm512_stream_si512(dst_p + 0 * 64, x0);
		_mm512_stream_si512(dst_p + 1 * 64, x1);
		_mm512_stream_si512(dst_p + 2 * 64, x2);
		_mm512_stream_si512(dst_p + 3 * 64, x3);
		_mm512_stream_si512(dst_p + 4 * 64, x4);
		_mm512_stream_si512(dst_p + 5 * 64, x5);
		_mm512_stream_si512(dst_p + 6 * 64, x6);
		_mm512_stream_si512(dst_p + 7 * 64, x7);

		src_p += 64 * 8;
		dst_p += 64 * 8;
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
