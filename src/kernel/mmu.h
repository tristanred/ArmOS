#ifndef MMU_H
#define MMU_H

#include <stdint.h>

#include "types.h"

enum GRSZ {
    GRAN4,
    GRAN16,
    GRAN64
};

void setup_mmu();

uint64_t* alloc_range(uint64_t start, uint64_t size, uint64_t alignment_bits);

uint64_t read_tcr();

void write_tcr(uint64_t value);

void enable_mmu();
void disable_mmu();

BOOL granule_supported(enum GRSZ value);

#endif