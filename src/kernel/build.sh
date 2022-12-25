#!/bin/sh

# Compile boot
aarch64-elf-as -c boot.S -o boot.o

# Compile kernel
aarch64-elf-gcc -ffreestanding -c kernel.c -o kernel.o -O0 -Og -g3 -Wall -Wextra


# Link everything together
aarch64-elf-gcc -T linker.ld -o myos.elf -ffreestanding -Og -nostdlib boot.o kernel.o -lgcc
aarch64-elf-objcopy myos.elf -O binary kernel8.img

# qemu-system-aarch64 -M raspi3b -kernel kernel8.img -nographic -s -S