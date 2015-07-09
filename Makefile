#------------------------------------------------------------------------------
# This script is adapted from http://preshing.com/20141119/how-to-build-a-gcc-cross-compiler
# qemu is used by default and compiled for i686-elf architecture.
#
# Main Targets:
# install
# build
#------------------------------------------------------------------------------

INSTALL_PATH=./compiler
PATH := $(INSTALL_PATH)/bin:$(PATH)
AS = i686-elf-as
GPLUS = i686-elf-g++
GCC = i686-elf-gcc
TOOLS = $(AS) $(GPLUS) $(GCC)
TARGET = dego

default: $(TARGET).iso

play: $(TARGET).iso
	qemu-system-i386 -cdrom $(TARGET).iso

clean_compiler: clean
	rm -rf compiler/

compiler_built.txt:
	./build_gcc_cross.sh

boot.o: boot.s compiler_built.txt
	compiler/bin/i686-elf-as boot.s -o boot.o

kernel.o: kernel.cpp compiler_built.txt
	compiler/bin/i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra 

$(TARGET).bin: boot.o kernel.o
	compiler/bin/i686-elf-gcc -T linker.ld -o $(TARGET).bin -ffreestanding -O2 -nostdlib boot.o kernel.o

isodir/boot/$(TARGET).bin: $(TARGET).bin
	mkdir -p isodir/boot
	cp $(TARGET).bin isodir/boot/$(TARGET).bin

isodir/boot/grub/grub.cfg: grub.cfg
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg

$(TARGET).iso: $(TARGET).bin isodir/boot/$(TARGET).bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET).iso isodir

clean: $(TARGET).iso
	rm *.o
	rm $(TARGET).bin
	rm isodir/boot/$(TARGET).bin
	rm isodir/boot/grub/grub.cfg
	rm $(TARGET).iso
