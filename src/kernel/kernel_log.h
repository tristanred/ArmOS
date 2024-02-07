#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

void kprintf(const char* str, ...);

#endif