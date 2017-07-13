#pragma once

#ifndef OSDEP_H_
#define OSDEP_H_

#include <cstddef>

unsigned numa_node_count();

unsigned numa_node_processor_count(unsigned node);

void *numa_malloc(unsigned node, size_t size, size_t alignment);

void numa_free(void *ptr);

void set_processor_affinity(unsigned node, unsigned n);

#endif // OSDEP_H_
