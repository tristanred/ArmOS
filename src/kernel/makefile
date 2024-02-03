### COMPILERS ###
AS=aarch64-elf-as
CC=aarch64-elf-gcc

### FLAGS ###
ASFLAGS=-g
CFLAGS=-ffreestanding -nostdlib -nostartfiles -g3 -Og -O0 -Wall -Wextra
LFLAGS=-ffreestanding -Og -nostdlib -nostartfiles

### DIRECTORIES ###
BUILDDIR=build/
OUTDIR=out/

all: build

prepare:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OUTDIR)

build: prepare bootloader kernel linker
	aarch64-elf-objcopy $(OUTDIR)myos.elf -O binary $(OUTDIR)myos.img

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
	qemu-system-aarch64 -M virt -cpu cortex-a76 -kernel $(OUTDIR)myos.img -nographic -s -S -d int

run: build
	qemu-system-aarch64 -M raspi3b -kernel $(OUTDIR)myos.img -nographic