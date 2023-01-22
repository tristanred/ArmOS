#ifndef INTERRUPT_H
#define INTERRUPT_H

void exc_handler(unsigned long type, unsigned long esr, unsigned long elr, unsigned long spsr, unsigned long far);

#endif