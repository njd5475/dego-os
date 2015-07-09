#!/usr/bin/env sh
#rm *.o *.bin *.iso
i686-elf-as boot.s -o boot.o
i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
#../../build-cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o
mkdir -p isodir/boot
cp myos.bin isodir/boot/myos.bin
grub-mkrescue -o myos.iso isodir
qemu-system-i386 -cdrom myos.iso
