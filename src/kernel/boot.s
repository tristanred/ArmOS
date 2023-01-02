.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // cpu id > 0, stop
1:  wfe
    b       1b
2:  // cpu id == 0

set_stack:
    // set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr     x1, =_start
    mov     sp, x1

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

    // jump to C code, should not return
4:  bl      kernel_main
    // for failsafe, halt this core too
    b       1b

drop_from_el3:
    adr x0, el2_start   // Read address of el2_start into x0
    msr ELR_EL3, x0     // Load start addr in the ELR register

    mov x0, 0b1001      // Set bits 0 and 3
    msr SPSR_EL3, x0    // Set SPSR to indicate using SP stored in SP_EL2

    mov x0, sp          // Store out current stack pointer in x0
    msr SP_EL2, x0      // Point EL2's SP to our current EL3 SP

    eret                // Return out of EL3 into EL2

el2_start:
    //mrs x0, CurrentEL
    //bl kernel_main
    bl test_test

test_test:
    ldr x0, 0
