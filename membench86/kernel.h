#pragma once

#ifndef KERNEL_H_
#define KERNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned read_memory_c(void *buf, size_t count);

unsigned read_memory_sse2(void *buf, size_t count);

unsigned read_memory_avx2(void *buf, size_t count);

unsigned read_memory_avx512(void *buf, size_t count);

unsigned write_memory_c(void *buf, size_t count);

unsigned write_memory_sse2(void *buf, size_t count);

unsigned write_memory_avx2(void *buf, size_t count);

unsigned write_memory_avx512(void *buf, size_t count);

unsigned write_memory_nt_sse2(void *buf, size_t count);

unsigned write_memory_nt_avx2(void *buf, size_t count);

unsigned write_memory_nt_avx512(void *buf, size_t count);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* KERNEL_H_ */
