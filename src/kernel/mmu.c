#include "mmu.h"

#include "sysregs/id_aa64mmfr0_el1.h"
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
    uint64_t capacities = 0;

    asm volatile("mrs %0, ID_AA64MMFR0_EL1" : "=r"(capacities));

    // Granule 4k and 64k indicate "not supported" by containing 0xF, granule
    // 16k by containing 0.
    switch (value) {
        case GRAN4: {
            return read_id_aa64mmfr0_el1().tgran4 != 0xF;
        }
        case GRAN16: {
            return read_id_aa64mmfr0_el1().tgran16 != 0x0;
        }
        case GRAN64: {
            return read_id_aa64mmfr0_el1().tgran64 != 0xF;
        }
        default:
            return FALSE;
    }
}