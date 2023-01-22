#include "mmu.h"

#include "sysregs/sctlr_el1.h"
#include "sysregs/tcr_el1.h"

uint64_t read_tcr() {
    uint64_t val = 0;

    asm volatile("mrs %0, TCR_EL1" : "=r"(val));

    return 0;
}

void write_tcr(uint64_t value) {
    asm volatile("msr TCR_EL1, %0" : : "r"(value));
}

void enable_mmu() {
    safe_write_sctlr_el1(.m = 1);
}

void disable_mmu() {
    safe_write_sctlr_el1(.m = 0);
}

BOOL granule_supported(enum GRSZ value) {

    uint64_t val = 0;

    asm volatile("mrs %0, ID_AA64MMFR0_EL1" : "=r"(val));

    // Granule 4k and 64k indicate "not supported" by containing 0xF, granule
    // 16k indicates by containing 0.
    switch (value) {
        case GRAN4: {
            uint64_t gran4_value = (value & 0xF0000000) >> 28;

            return gran4_value != 0xF;
        }
        case GRAN16: {
            uint64_t gran16_value = (value & 0xF00000) >> 20;

            return gran16_value != 0xF;
        }
        case GRAN64: {
            uint64_t gran64_value = (value & 0xF000000) >> 24;

            return gran64_value != 0;
        }
        default:
            return FALSE;
    }
}