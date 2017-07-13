#pragma once

#ifndef KERNEL_H_
#define KERNEL_H_

unsigned read_memory_c(const void *buf, size_t count);

unsigned read_memory_sse2(const void *buf, size_t count);

unsigned read_memory_avx2(const void *buf, size_t count);

#endif // KERNEL_H_
