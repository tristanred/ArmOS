#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

/**
 * Kernel memory overview
 *
 * This file contains methods commonly used for managing kernel memory. This
 * type of memory is more "raw" than normal memory. Allocations are done
 * right into the RAM without regard for memory management. Memory of this
 * type is used for kernel modules that are typically allocated once when the
 * kernel boots and never really moves around.
 */

#include <stdint.h>

struct m_heap_allocation {
    uint64_t addr;
    uint64_t size;

    struct m_heap_allocation* next;
};

struct m_heap {
    uint64_t start;
    uint64_t end;

    struct m_heap_allocation* first_alloc;
};

/// @brief Allocate a block of kernel memory on a heap.
/// @param size Size of the allocation in bytes.
/// @param heap Heap structure to allocate within.
/// @return Pointer to the allocated object. NULL if allocation failed.
void* kmalloc(uint32_t size, struct m_heap* heap);

/// @brief Free a block of kernel memory from a heap. Since kernel memory is
/// mostly 'unmanaged', we don't have the capacity to allocate data within freed
// spaces of memory left by freeing memory. This function will be mostly unused.
/// @param alloc Allocation object to free.
/// @param heap Heap to free from.
void kfree(void* alloc, struct m_heap* heap);

#endif