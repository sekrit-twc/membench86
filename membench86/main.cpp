#include <algorithm>
#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
#include "argparse.h"
#include "kernel.h"
#include "osdep.h"
#include "timer.h"

namespace {

constexpr unsigned long long operator ""_kB(unsigned long long x) { return x * (1ULL << 10); }
constexpr unsigned long long operator ""_GB(unsigned long long x) { return x * (1ULL << 30); }

enum class AccessMode {
	READ,
	WRITE,
	COPY,
};

enum class MemorySharing {
	PRIVATE,
	SHARED,
	OVERLAPPED,
};

int arg_decode_mode(const struct ArgparseOption *, void *out, const char *param, int)
{
	AccessMode *mode = static_cast<AccessMode *>(out);

	if (!std::strcmp(param, "READ") || !std::strcmp(param, "read"))
		*mode = AccessMode::READ;
	else if (!std::strcmp(param, "WRITE") || !std::strcmp(param, "write"))
		*mode = AccessMode::WRITE;
	else if (!std::strcmp(param, "COPY") || !std::strcmp(param, "copy"))
		*mode = AccessMode::COPY;
	else
		return 1;

	return 0;
}

int arg_decode_sharing(const struct ArgparseOption *, void *out, const char *param, int)
{
	MemorySharing *sharing = static_cast<MemorySharing *>(out);

	if (!std::strcmp(param, "PRIVATE") || !std::strcmp(param, "private"))
		*sharing = MemorySharing::PRIVATE;
	else if (!std::strcmp(param, "SHARED") || !std::strcmp(param, "shared"))
		*sharing = MemorySharing::SHARED;
	else if (!std::strcmp(param, "OVERLAPPED") || !std::strcmp(param, "overlapped"))
		*sharing = MemorySharing::OVERLAPPED;
	else
		return 1;

	return 0;
}

struct Arguments {
	AccessMode mode = AccessMode::READ;
	MemorySharing sharing = MemorySharing::PRIVATE;
	unsigned long long size_kb = 1024 + 256;
	unsigned long long total_size_gb = 1024;
	unsigned num_nodes = 0;
	unsigned num_cpus = 0;
	char hugepage = 1;
	char sse = 0;
	char avx = 0;
	char avx512 = 0;
};

constexpr ArgparseOption program_switches[] = {
	{ OPTION_USER1,     "m", "mode",        offsetof(Arguments, mode),          arg_decode_mode, "access mode (READ/WRITE/COPY)" },
	{ OPTION_USER1,     "s", "sharing",     offsetof(Arguments, sharing),       arg_decode_sharing, "data sharing (PRIVATE/SHARED/OVERLAPPED)" },
	{ OPTION_ULONGLONG, "b", "buffer-size", offsetof(Arguments, size_kb),       nullptr, "per-thread buffer size (kB)" },
	{ OPTION_ULONGLONG, "t", "total-size",  offsetof(Arguments, total_size_gb), nullptr, "per-thread total memory accessed (GB)" },
	{ OPTION_UINT,      "n", "num-nodes",   offsetof(Arguments, num_nodes),     nullptr, "number of NUMA nodes to use" },
	{ OPTION_UINT,      "c", "num-cpus",    offsetof(Arguments, num_cpus),      nullptr, "number of CPUs per node to use" },
	{ OPTION_FLAG,      "p", "hugepage",    offsetof(Arguments, hugepage),      nullptr, "enable huge pages" },
	{ OPTION_FLAG,      "x", "sse",         offsetof(Arguments, sse),           nullptr, "use SSE2 (XMM) kernel" },
	{ OPTION_FLAG,      "y", "avx",         offsetof(Arguments, avx),           nullptr, "use AVX2 (YMM) kernel" },
#ifdef __INTEL_COMPILER
	{ OPTION_FLAG,      "z", "avx512",      offsetof(Arguments, avx512),        nullptr, "use AVX-512 (ZMM) kernel" },
#endif
	{ OPTION_NULL }
};

constexpr ArgparseCommandLine program_def = {
	program_switches,
	nullptr,
	"membench86",
	"Measure memory bandwidth",
};


class Worker {
	std::shared_ptr<const void> m_buffer;
	std::atomic_bool *m_start_flag;
	std::atomic_bool *m_cancel_flag;
	unsigned m_numa_node;
	unsigned m_cpu_num;
	unsigned m_iter_count;

	unsigned (*m_kernel)(const void *, size_t);

	size_t m_first;
	size_t m_last;
public:
	Worker(std::shared_ptr<const void> buffer, std::atomic_bool *start_flag, std::atomic_bool *cancel_flag, unsigned numa_node, unsigned cpu_num) :
		m_buffer{ std::move(buffer) },
		m_start_flag{ start_flag },
		m_cancel_flag{ cancel_flag },
		m_numa_node{ numa_node },
		m_cpu_num{ cpu_num },
		m_iter_count{},
		m_kernel{},
		m_first{},
		m_last{}
	{}

	void set_iter_count(unsigned iter_count) { m_iter_count = iter_count; }

	void set_kernel(unsigned (*kernel)(const void *, size_t)) { m_kernel = kernel; }

	void set_bounds(size_t first, size_t last) { m_first = first; m_last = last; }

	void operator()()
	{
		const void *buf = (const char *)m_buffer.get() + m_first;
		size_t count = m_last - m_first;

		set_processor_affinity(m_numa_node, m_cpu_num);

		do {
			// Run kernel while waiting to ensure YMM/ZMM state is initialized.
			m_kernel(buf, std::min(count, static_cast<size_t>(64)));
		} while (!*m_start_flag && !*m_cancel_flag);

		for (unsigned i = 0; i < m_iter_count; ++i) {
			if (i % 1024 == 0 && *m_cancel_flag)
				return;

			volatile unsigned x = m_kernel(buf, count);
		}
	}
};

void run(const Arguments &args)
{
	if (args.mode != AccessMode::READ)
		throw std::runtime_error{ "only READ access supported" };

	// Get physical processor count.
	unsigned num_nodes = args.num_nodes ? args.num_nodes : numa_node_count();
	unsigned num_cpus = 0;

	// Allocate memory.
	std::vector<size_t> allocated_size(num_nodes);
	std::vector<std::shared_ptr<void>> buffers(num_nodes);
	for (unsigned i = 0; i < num_nodes; ++i) {
		unsigned num_node_cpus = args.num_cpus ? args.num_cpus : numa_node_processor_count(i);

		switch (args.sharing) {
		case MemorySharing::PRIVATE:
			allocated_size[i] = static_cast<size_t>(args.size_kb * 1_kB * num_node_cpus);
			break;
		case MemorySharing::SHARED:
			allocated_size[i] = static_cast<size_t>(args.size_kb * 1_kB);
			break;
		case MemorySharing::OVERLAPPED:
			allocated_size[i] = static_cast<size_t>(((args.size_kb * 1_kB) / 2) * num_node_cpus + args.size_kb * 1_kB);
			break;
		}

		if (args.hugepage) {
			buffers[i] = std::shared_ptr<void>{ numa_malloc_hugepage(i, allocated_size[i]), numa_free };
			if (!buffers[i])
				std::cerr << "failed to allocate huge page\n";
		}
		if (!buffers[i]) {
			buffers[i] = std::shared_ptr<void>{ numa_malloc(i, allocated_size[i]), numa_free };
			if (!buffers[i])
				throw std::bad_alloc{};
		}

		std::memset(buffers[i].get(), 0xFF, allocated_size[i]);
		num_cpus += num_node_cpus;
	}

	// Initialize workers.
	std::vector<std::thread> threads;
	threads.reserve(num_cpus);

	std::atomic_bool start_flag{ false };
	std::atomic_bool cancel_flag{ false };

	try {
		for (unsigned node_idx = 0; node_idx < num_nodes; ++node_idx) {
			unsigned num_node_cpus = args.num_cpus ? args.num_cpus : numa_node_processor_count(node_idx);

			for (unsigned cpu_idx = 0; cpu_idx < num_node_cpus; ++cpu_idx) {
				Worker w{ buffers[node_idx], &start_flag, &cancel_flag, node_idx, cpu_idx };

				w.set_iter_count(static_cast<unsigned>((args.total_size_gb * 1_GB) / (args.size_kb * 1_kB)));

				switch (args.mode) {
				case AccessMode::READ:
#ifdef __INTEL_COMPILER
					if (args.avx512)
						w.set_kernel(read_memory_avx512);
					else
#endif
					if (args.avx)
						w.set_kernel(read_memory_avx2);
					else if (args.sse)
						w.set_kernel(read_memory_sse2);
					else
						w.set_kernel(read_memory_c);
					break;
				default:
					return;
				}

				switch (args.sharing) {
				case MemorySharing::PRIVATE:
					w.set_bounds(
						static_cast<size_t>(args.size_kb * 1_kB * cpu_idx),
						static_cast<size_t>(args.size_kb * 1_kB * (cpu_idx + 1)));
					break;
				case MemorySharing::SHARED:
					w.set_bounds(0, allocated_size[node_idx]);
					break;
				case MemorySharing::OVERLAPPED:
					w.set_bounds(
						static_cast<size_t>(((args.size_kb * 1_kB) / 2) * cpu_idx),
						static_cast<size_t>(((args.size_kb * 1_kB) / 2) * cpu_idx + args.size_kb * 1_kB));
					break;
				}

				threads.emplace_back(std::move(w));
			}
		}
	} catch (...) {
		cancel_flag = true;

		for (auto &th : threads) {
			th.join();
		}
	}

	// Start timing.
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	Timer timer;
	timer.start();

	start_flag = true;
	for (auto &th : threads) {
		th.join();
	}

	timer.stop();

	// Print report.
	double bandwidth_gbps = args.total_size_gb * num_cpus / timer.elapsed();
	std::cout << "elapsed: " << timer.elapsed() << " s\n";
	std::cout << "bandwidth: " << bandwidth_gbps << " GB/s\n";
}

} // namespace


int main(int argc, char **argv)
{
	Arguments args{};

	int ret = argparse_parse(&program_def, &args, argc, argv);
	if (ret < 0)
		return ret == ARGPARSE_HELP_MESSAGE ? 0 : 2;

	try {
		run(args);
	} catch (const std::exception &e) {
		std::cerr << "C++ exception: " << e.what() << '\n';
		return 1;
	} catch (...) {
		std::cerr << "unknown error\n";
		return 1;
	}

	return 0;
}
