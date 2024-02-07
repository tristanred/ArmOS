#include "kernel_log.h"

#include "string.h"
#include "uart.h"

void kprintf(const char* str, ...) {
    char outbuf[256];

    va_list formats;
    va_start(formats, str);
    vsprintf(outbuf, str, formats);
    va_end(formats);

    uart_puts(outbuf);
    uart_puts("\n");
}