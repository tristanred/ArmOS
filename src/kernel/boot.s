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

// Entrypoint of CPU 0 once all other cores have stopped.
cpu_one_start:
    mrs     x0, CurrentEL

// Hook just before we change our stack pointer
set_stack:
    // set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr     x1, =_start
    mov     sp, x1
// Hook just after we change our stack pointer
after_stack_change:

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, drop_to_el1 // Jump forward here, for some reason.
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

// Drop however many levels needed to go to EL1 (OS)
drop_to_el1:
check_el3:
    mrs x0, CurrentEL
    cmp x0, #0b1100
    bne check_el2
    bl drop_from_el3
check_el2:
    mrs x0, CurrentEL
    cmp x0, #0b1000
    bne kernel_handoff
    bl drop_from_el2

    b kernel_handoff


// ELR = Exception Link Register. The address to return to from an exception
// level. Ex: calling eret from EL2 will go to address ELR_EL2
drop_from_el3:
    adr x0, drop_to_el1 // Read address of target return function in x0
    msr ELR_EL3, x0     // Load start addr in the ELR register

    mov x0, 0b1001      // Set bits 0 and 3
    msr SPSR_EL3, x0    // Set SPSR to indicate using SP stored in SP_EL2

    // At the moment, we keep stack pointers between EL1, EL2 and EL3
    mov x0, sp          // Store out current stack pointer in x0
    msr SP_EL2, x0      // Point EL2's SP to our current EL3 SP

    eret                // Return out of EL3 into EL2

drop_from_el2:
    adr x0, drop_to_el1 // Read address of target return function in x0
    msr ELR_EL2, x0     // Load target function address in ELR_EL2

    mov x0, 0b0101      // Set bits 0 and 3
    msr SPSR_EL2, x0    // Set SPSR to indicate using SP stored in SP_EL2

    // At the moment, we keep stack pointers between EL1, EL2 and EL3
    mov x0, sp          // Store out current stack pointer in x0
    msr SP_EL1, x0      // Point EL1's SP to our current EL2 SP

    eret

    // jump to C code, should not return
kernel_handoff:
    bl      kernel_main
    // for failsafe, halt this core too
    b       1b