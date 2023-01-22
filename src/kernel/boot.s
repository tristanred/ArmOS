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

    // set up exception handlers
    ldr x0, =interrupt_vectors_el1
    msr VBAR_EL1, x0

    ldr x0, =interrupt_vectors_el2
    msr VBAR_EL2, x0

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

drop_from_el3:
    // ELR = Exception Link Register. The address to return to from an exception
    // level. Ex: calling eret from EL2 will go to address ELR_EL2
    adr x0, drop_to_el1 // Read address of target return function in x0
    msr ELR_EL3, x0     // Load start addr in the ELR register

    mov x0, 0b1001      // No idea why I need to do this
    msr SPSR_EL3, x0

    mov x0, #0x70000    // Set EL2 stack pointer
    msr SP_EL2, x0

    eret                // Return out of EL3 into EL2

drop_from_el2:
    adr x0, drop_to_el1 // Read address of target return function in x0
    msr ELR_EL2, x0     // Load target function address in ELR_EL2

    mov x0, 0b0101      // No idea why I need to do this
    msr SPSR_EL2, x0

    mov x0, #0x70000
    msr SP_EL1, x0      // Set EL1 stack pointer

    eret

    // jump to C code, should not return
kernel_handoff:

    bl      kernel_main
    // for failsafe, halt this core too
    b       1b

    .balign 4096
interrupt_vectors_el1:
    // ## Section 1 : Exception from the current EL while using SP_EL0 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // ## Section 2 : Exception from the current EL while using SP_ELx ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // ## Section 3 : Exception from a lower EL and at least one lower EL is AArch64 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // ## Section 4 : Exception from a lower EL and at least one lower EL is AArch64 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el1
    mrs     x2, elr_el1
    mrs     x3, spsr_el1
    mrs     x4, far_el1
    b       exc_handler

    .balign 4096
interrupt_vectors_el2:
    // ## Section 1 : Exception from the current EL while using SP_EL0 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // ## Section 2 : Exception from the current EL while using SP_ELx ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // ## Section 3 : Exception from a lower EL and at least one lower EL is AArch64 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // ## Section 4 : Exception from a lower EL and at least one lower EL is AArch64 ##
    // synchronous
    .align  7
    mov     x0, #0
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // IRQ
    .align  7
    mov     x0, #1
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // FIQ
    .align  7
    mov     x0, #2
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler

    // SError
    .align  7
    mov     x0, #3
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    mrs     x3, spsr_el2
    mrs     x4, far_el2
    b       exc_handler