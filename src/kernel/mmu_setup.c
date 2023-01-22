#include "mmu_setup.h"

#include "mmu.h"
#include "types.h"

void setup_mmu() {
    // Steps needed
    // 1. Check if we support 4k granule
    // 2. Setup `mair_el1` (Memory Attribute Indirection Register)
    // 3. Setup `tcr_el1` (Translation Control Register)
    // 4. Setup `ttbr0_el1` and `ttbr1_el1`
    // 5. Setup `SCTLR_EL1` (System Control Register)

    if (granule_supported(GRAN4) == FALSE) {
        
    }
}