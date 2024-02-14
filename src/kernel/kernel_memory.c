#include "kernel_memory.h"

#include "types.h"
#include <stddef.h>

void* kmalloc(uint32_t size, struct m_heap* heap) {
    // Check if alloc is larger than heap size
    if (size > heap->end - heap->start) {
        return NULL;
    }

    /**
     * General strategy for managing allocations without needing to allocate
     * memory for tracking.
     *
     * Each allocation structure and its data is added one after the other. It
     * looks like this:
     *
     * 0x12345            0x12354                    0xAB1234          0xAB1243
     * +--------------------+---------------------------+--------------------+
     * |    Alloc Struct 1  |         Alloc Data        |   Alloc Struct 2   |
     * +--------------------+---------------------------+--------------------+
     *
     * This allows us to avoid any any dynamic allocations. Each alloc struct
     * is initialized in place right after the previous's allocation data. We
     * have to scan the whole list of allocation to reach the end each time but
     * it's a minor hit.
     */

    if (heap->first_alloc == NULL) {
        uint64_t first_alloc_addr = (uint64_t)heap->first_alloc;

        struct m_heap_allocation* alloc =
            (struct m_heap_allocation*)first_alloc_addr;

        // Set the allocation to sit right after the alloc structure
        alloc->addr = (uint64_t)alloc + sizeof(struct m_heap_allocation);
        alloc->size = size;
        alloc->next = NULL;

        return alloc;
    }

    // Iterate until we reach the allocation that doesn't have a "next"
    struct m_heap_allocation* next =
        (struct m_heap_allocation*)heap->first_alloc;
    while (next->next != NULL) {
        next = next->next;
    }

    struct m_heap_allocation* alloc =
        (struct m_heap_allocation*)(next->addr + next->size);
    alloc->addr = (uint64_t)alloc + sizeof(struct m_heap_allocation);
    alloc->size = size;
    alloc->next = NULL;

    return alloc;
}

void kfree(void* alloc, struct m_heap* heap) {
    // Bail out if we don't have any allocations.
    if (heap->first_alloc == NULL) {
        return;
    }

    // Special case for the first element. If we delete it, link it's next
    // element as the first. If it's null, then we removed the first and only
    // element, don't need to do anything else.
    if (heap->first_alloc == alloc) {
        heap->first_alloc = heap->first_alloc->next;
    }

    // Go through each element and save the previous. If the element we are
    // iterating on matches the alloc we want to free, link the previous with
    // our current's next.
    struct m_heap_allocation* previous = heap->first_alloc;
    struct m_heap_allocation* current = previous->next;

    while (current != NULL) {
        if (current == alloc) {
            previous->next = current->next;

            break;
        }
    }
}
