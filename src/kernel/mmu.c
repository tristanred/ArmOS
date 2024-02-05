#include "mmu.h"

#include "sysregs/id_aa64mmfr0_el1.h"
#include "sysregs/sctlr_el1.h"
#include "sysregs/tcr_el1.h"

const int RAM_START = 0x0;
const int RAM_END = 0x3f000000;
const int RAM_SZ = RAM_END - RAM_START;

const uint64_t ALIGN_FLAG_4KB = 0xFFF;
const uint64_t ALIGN_FLAG_16KB = 0x3FFF;
const uint64_t ALIGN_FLAG_64KB = 0xFFFF;

void setup_mmu() {

    // Usable ram goes from 0x0 to 000000003f000000. The range for devices is
    // 000000003f000000-000000003fffffff

    int* ram_start = (int*)0x0;
    int* ram_end = (int*)0x3f000000;

    int ram_start_value = *ram_start;
    int ram_end_value = *ram_end;

    // Report on each granule support
    BOOL gran4_supported = granule_supported(GRAN4);
    BOOL gran16_supported = granule_supported(GRAN16);
    BOOL gran64_supported = granule_supported(GRAN64);

    union tcr_el1 tcr = read_tcr_el1();

    // Write values
    // T0SZ = 32. Amount of bits to shift the input address by. (No idea what
    // this is about) TG0 = 0x0. Granule size for the first level of translation
    // table. 0x0 = 4KB IPS = 0x0. Intermediate Physical Address Size. 0x0 = 32
    // bits
    safe_write_tcr_el1(.t0sz = 32, .tg0 = 0x0, .ips = 0x0);

    // Each table is stored contiguously. So for example:
    // L1 L1 L1 L1 -> L2 L2 L2 L2 -> L3 L3 L3 L3

    // TODO: Current issue, each table need to be aligned on a 4K (granule size)
    // boundary.

    // We need 1 L1 table with 4 entries, each entry points to an L2 table.
    uint64_t l1_table_size = 4 * sizeof(uint64_t);
    uint64_t* l1_table = alloc_range(0x0, l1_table_size, ALIGN_FLAG_4KB);

    // We need 4 L2 tables with 512 entries each.
    uint64_t l2_table_size = 512 * 4 * sizeof(uint64_t);
    uint64_t* l2_table =
        alloc_range(l1_table + l2_table_size, l2_table_size, ALIGN_FLAG_4KB);

    // We need 512 L3 tables with 512 entries each.
    uint64_t l3_table_size = 512 * 512 * sizeof(uint64_t);
    uint64_t* l3_table =
        alloc_range(l2_table + l2_table_size, l3_table_size, ALIGN_FLAG_4KB);

    // Setup the L1 table
    for (int i = 0; i < 4; i++) {
        // Each entry stores the address of an L2 table.
        l1_table[i] = (uint64_t)l2_table + (i * 512 * sizeof(uint64_t));
    }

    for (int i = 0; i < 512; i++) {
    }
}

uint64_t* alloc_range(uint64_t start, uint64_t size, uint64_t alignment_bits) {
    return (uint64_t*)(RAM_START + (start & ~(alignment_bits)));
}

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
            return read_id_aa64mmfr0_el1().tgran4 == 0x0;
        }
        case GRAN16: {
            return read_id_aa64mmfr0_el1().tgran16 == 0x1;
        }
        case GRAN64: {
            return read_id_aa64mmfr0_el1().tgran64 == 0x0;
        }
        default:
            return FALSE;
    }
}