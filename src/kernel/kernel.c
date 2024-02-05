#include <stddef.h>
#include <stdint.h>

#include "mmu.h"
#include "types.h"
#include "uart.h"

int test(int a, int b, char* test) {
    int c = a + b;

    return c;
}

// arguments for AArch64
void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3) {

    int res = test(1, 2, "test");

    setup_mmu();

    // initialize UART for Raspi2
    uart_init(3);
    uart_puts("Hello, kernel World!\r\n");

    setup_mmu();

    while (1) {
        uart_putc(uart_getc());
    }
}
