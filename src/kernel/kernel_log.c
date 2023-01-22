#include "kernel_log.h"

#include "uart.h"

void kprintf(const char* buffer) {
    uart_puts(buffer);
}
