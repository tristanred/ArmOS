#include "mmu.h"

#include "kernel_log.h"
#include "sysregs/id_aa64mmfr0_el1.h"
#include "sysregs/sctlr_el1.h"
#include "sysregs/tcr_el1.h"
#include "sysregs/ttbr0_el1.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

const int RAM_START = 0x1000; // Start at 4KB
const int RAM_END = 0x3f000000;
const int RAM_SZ = RAM_END - RAM_START;

const uint64_t ALIGN_FLAG_4KB = 0xFFF;
const uint64_t ALIGN_FLAG_16KB = 0x3FFF;
const uint64_t ALIGN_FLAG_64KB = 0xFFFF;

void setup_mmu() {

    // Usable ram goes from 0x0 to 000000003f000000. The range for devices is
    // 000000003f000000-000000003fffffff

    int ram_start_value = RAM_START;
    int ram_end_value = RAM_END;

    // Report on each granule support
    BOOL gran4_supported = granule_supported(GRAN4);
    BOOL gran16_supported = granule_supported(GRAN16);
    BOOL gran64_supported = granule_supported(GRAN64);

    kprintf("Granule 4KB supported: %b\n", gran4_supported);
    kprintf("Granule 16KB supported: %b\n", gran16_supported);
    kprintf("Granule 64KB supported: %b\n", gran64_supported);

    union tcr_el1 tcr = read_tcr_el1();
    (void)tcr;

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

    // The below tables, the pointer to each table is a pointer to the first
    // element of the table.That's why we don't need a uint64_t**. That also
    // means that we can't use the [] operator to access the elements of the
    // table.

    // We need 1 L1 table with 4 entries, each entry points to an L2 table.
    // This table needs to be 4K aligned
    uint64_t l1_table_size = 4 * sizeof(uint64_t);
    uint64_t* l1_table =
    alloc_range((uint64_t)0x0, l1_table_size, ALIGN_FLAG_4KB);

    // We need 4 L2 tables with 512 entries each.
    // Each table needs to be table aligned
    uint64_t l2_table_size = 512 * 4 * sizeof(uint64_t);
    uint64_t* l2_table = alloc_range((uint64_t)l1_table + l2_table_size,
                                     l2_table_size, ALIGN_FLAG_4KB);

    // We need 512 L3 tables with 512 entries each.
    uint64_t l3_table_size = 512 * 512 * 4 * sizeof(uint64_t);
    uint64_t* l3_table = alloc_range((uint64_t)l2_table + l2_table_size,
                                     l3_table_size, ALIGN_FLAG_4KB);

    // Setup the L1 table entries
    for (int i = 0; i < 4; i++) {
        // Each entry stores the address of an L2 table.
        uint64_t addr_flags = (uint64_t)l2_table + (i * 512 * sizeof(uint64_t));

        l1_table[i] = addr_flags | 0b11;
    }

    // For each entry we have populated, create 512 entries in the L2 table
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 512; k++) {
            uint64_t table_index = i;
            (void)table_index;

            uint64_t entry_index = k + (i * 512);

            uint64_t addr_flags =
                (uint64_t)l3_table + (k * 512 * sizeof(uint64_t));

            l2_table[entry_index] = addr_flags;
        }
    }

    // Populate the 2048 L3 tables and 1048576 entries
    for (int i = 0; i < 512 * 4; i++) {
        for (int k = 0; k < 512; k++) {
            uint64_t table_index = i;
            uint64_t entry_index = k + (i * 512);

            uint64_t addr_flags =
                (table_index << 21) | (entry_index << 12) | 0x3;

            l3_table[entry_index] = addr_flags;
        }
    }

    safe_write_ttbr0_el1(.baddr = (uint64_t)l1_table);

    safe_write_sctlr_el1(.m = 1, .c = 1, .i = 1);
}

uint64_t* alloc_range(uint64_t start, uint64_t size, uint64_t alignment_bits) {
    (void)size;
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

    // Granule 4k and 64k indicate "not supported" by containing 0xF,
    // granule 16k by containing 0.
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