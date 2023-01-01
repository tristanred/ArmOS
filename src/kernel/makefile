### COMPILERS ###
AS=aarch64-elf-as
CC=aarch64-elf-gcc

### FLAGS ###
ASFLAGS=-g
CFLAGS=-std=gnu99 -ffreestanding -g3 -Og -O0 -Wall -Wextra
LFLAGS=-ffreestanding -Og -nostdlib -lgcc

### DIRECTORIES ###
BUILDDIR=build/
OUTDIR=out/

all: build

prepare:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OUTDIR)

build: prepare bootloader kernel linker
	aarch64-elf-objcopy $(OUTDIR)myos.elf -O binary $(OUTDIR)kernel8.img

bootloader: prepare boot.s
	$(AS) -c boot.s -o $(BUILDDIR)boot.o $(ASFLAGS)

kernel: prepare
	$(CC) -c *.c $(CFLAGS)
	mv *.o $(BUILDDIR)

linker: prepare kernel bootloader
	$(CC) -T linker.ld -o $(OUTDIR)myos.elf $(BUILDDIR)*.o $(LFLAGS)

.PHONY: clean
clean:
	rm -rf $(OUTDIR) $(BUILDDIR)

debug: build
	qemu-system-aarch64 -M raspi3b -kernel $(OUTDIR)myos.elf -nographic -s -S

run: build
	qemu-system-aarch64 -M raspi3b -kernel $(OUTDIR)myos.elf -nographic