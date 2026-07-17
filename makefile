CC = gcc
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib

OBJS = kernel.o vga.o keyboard.o ata.o idt.o inter.o timer.o gdt.o win.o calcul.o

all: build

boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

inter.o: inter.asm
	nasm -f elf32 inter.asm -o inter.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	ld -m elf_i386 -T link.ld -o kernel.elf $(OBJS)

kernel.bin: kernel.elf
	objcopy -O binary kernel.elf kernel.bin

disk:
	dd if=/dev/zero of=disk.img bs=512 count=65536
	mkfs.fat -F 16 -R 64 disk.img
	echo "I am a hacker" > h.txt
	mcopy -i disk.img h.txt ::h.txt
	mcopy -i disk.img test400.ppm ::test400.ppm
	mcopy -i disk.img test600.bmp ::test600.bmp
	dd if=boot.bin of=disk.img bs=1 count=446 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

build: boot.bin kernel.bin
	dd if=boot.bin of=disk.img bs=1 count=446 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

run: build
	qemu-system-x86_64 -drive format=raw,file=disk.img -rtc base=localtime -vga std

clean:
	rm -f *.o *.elf *.bin

.PHONY: all run clean disk build
