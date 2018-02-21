#include "osdep.h"

#ifdef _WIN32
#include <memory>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>
#include <Windows.h>
#include "aligned_malloc.h"

namespace {

class PhysicalProcessorInformation {
	struct Node {
		DWORD node;
		std::vector<GROUP_AFFINITY> cpus;
	};

	std::vector<Node> m_info;
public:
	void add_numa_node(DWORD node) { m_info.emplace_back(Node{ node, {} }); }

	void add_cpu(unsigned node, GROUP_AFFINITY info) { m_info[node].cpus.emplace_back(info); }

	DWORD numa_number(unsigned node) const { return m_info[node].node; }

	GROUP_AFFINITY affinity_mask(unsigned node, unsigned n) const { return m_info[node].cpus[n]; }

	unsigned num_nodes() const { return static_cast<unsigned>(m_info.size()); }

	unsigned num_cpus(unsigned node) const { return static_cast<unsigned>(m_info[node].cpus.size()); }
};

[[noreturn]] void throw_system_error(const char *msg)
{
	throw std::system_error{ std::error_code{ static_cast<int>(GetLastError()), std::system_category() }, msg };
}

PhysicalProcessorInformation get_processor_information()
{
	PhysicalProcessorInformation info;

	std::unique_ptr<unsigned char[]> numa_info{
		new unsigned char[65536]
	};
	DWORD numa_info_size = 65536;

	if (!GetLogicalProcessorInformationEx(
		static_cast<LOGICAL_PROCESSOR_RELATIONSHIP>(RelationNumaNode),
		reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(numa_info.get()),
		&numa_info_size
	)) {
		throw_system_error("error querying NUMA topology");
	}
	for (unsigned char *ptr = numa_info.get(); ptr < numa_info.get() + numa_info_size;) {
		auto *record = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(ptr);
		info.add_numa_node(record->NumaNode.NodeNumber);
		ptr += record->Size;
	}

	std::unique_ptr<unsigned char[]> core_info{
		new unsigned char[65536]
	};
	DWORD core_info_size = 65536;

	if (!GetLogicalProcessorInformationEx(
		static_cast<LOGICAL_PROCESSOR_RELATIONSHIP>(RelationProcessorCore),
		reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(core_info.get()),
		&core_info_size
	)) {
		throw_system_error("error querying logical processor relationship");
	}
	for (unsigned char *ptr = core_info.get(); ptr < core_info.get() + core_info_size;) {
		auto *record = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(ptr);
		GROUP_AFFINITY affinity = record->Processor.GroupMask[0];
		unsigned long index = 0;

		// Select the lowest processor number.
#ifdef _M_X64
		_BitScanForward64(&index, affinity.Mask);
#else
		_BitScanForward(&index, affinity.Mask);
#endif
		affinity.Mask &= (1ULL << index);

		// Search for the NUMA node containing the group and mask.
		unsigned node = 0;
		{
			unsigned j = 0;
			for (unsigned char *ptr2 = numa_info.get(); ptr2 < numa_info.get() + numa_info_size;) {
				auto *record = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *>(ptr2);

				if (record->NumaNode.GroupMask.Group == affinity.Group &&
					(record->NumaNode.GroupMask.Mask & affinity.Mask))
				{
					node = j;
					break;
				}

				ptr += record->Size;
				++j;
			}
		}
		info.add_cpu(node, affinity);

		ptr += record->Size;
	}

	return info;
}

const PhysicalProcessorInformation &processor_information()
{
	static const PhysicalProcessorInformation info = get_processor_information();
	return info;
}

bool enable_hugepages()
{
	HANDLE token = INVALID_HANDLE_VALUE;
	TOKEN_PRIVILEGES priv = {};

	if (!(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)))
		return false;

	if (!(LookupPrivilegeValue(nullptr, SE_LOCK_MEMORY_NAME, &priv.Privileges[0].Luid))) {
		CloseHandle(token);
		return false;
	}

	priv.PrivilegeCount = 1;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!(AdjustTokenPrivileges(token, FALSE, &priv, 0, nullptr, 0))) {
		CloseHandle(token);
		return false;
	}

	CloseHandle(token);
	return true;
}

} // namespace


unsigned numa_node_count()
{
	return processor_information().num_nodes();
}

unsigned numa_node_processor_count(unsigned node)
{
	return processor_information().num_cpus(node);
}

void *numa_malloc(unsigned node, size_t size)
{
	return VirtualAllocExNuma(GetCurrentProcess(), nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, processor_information().numa_number(node));
}

void *numa_malloc_hugepage(unsigned node, size_t size)
{
	static const bool enabled = enable_hugepages();
	if (!enabled)
		return nullptr;

	static const size_t pagesize = GetLargePageMinimum();

	if (size % pagesize)
		size += pagesize - size % pagesize;

	return VirtualAllocExNuma(GetCurrentProcess(), nullptr, size, MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE, processor_information().numa_number(node));
}

void numa_free(void *ptr)
{
	VirtualFreeEx(GetCurrentProcess(), ptr, 0, MEM_RELEASE);
}

void set_processor_affinity(unsigned node, unsigned n)
{
	GROUP_AFFINITY affinity = processor_information().affinity_mask(node, n);

	if (!SetThreadGroupAffinity(GetCurrentThread(), &affinity, nullptr))
		std::terminate();
}
#else // _WIN32
#include "aligned_malloc.h"

unsigned numa_node_count()
{
	return 1;
}

unsigned numa_node_processor_count(unsigned node)
{
	return 1;
}

void *numa_malloc(unsigned node, size_t size)
{
	return aligned_malloc(size, 4096);
}

void *numa_malloc_hugepage(unsigned node, size_t size)
{
	return aligned_malloc(size, static_cast<size_t>(2 * (1UL << 20)));
}

void numa_free(void *ptr)
{
	aligned_free(ptr);
}

void enable_hugepage()
{
}

void set_processor_affinity(unsigned node, unsigned n)
{
}
#endif // _WIN32
