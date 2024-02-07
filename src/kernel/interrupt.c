#include "interrupt.h"

#include "kernel_log.h"
#include "sysregs/elr_el1.h"

void exc_handler(unsigned long type, unsigned long esr, unsigned long elr,
                 unsigned long spsr, unsigned long far) {
    kprintf("Received interrupt");
    (void)type;
    (void)esr;
    (void)elr;
    (void)spsr;
    (void)far;
}