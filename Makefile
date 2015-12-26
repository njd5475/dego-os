#------------------------------------------------------------------------------
# This script is adapted from http://preshing.com/20141119/how-to-build-a-gcc-cross-compiler
# qemu is used by default and compiled for i686-elf architecture.
#
# Main Targets:
# install
# build
#------------------------------------------------------------------------------

INSTALL_PATH=./compiler
PATH:=$(INSTALL_PATH)/bin/bin:$(PATH)
AS=i686-elf-as
GPLUS=i686-elf-g++
GCC=i686-elf-gcc
TOOLS=$(AS) $(GPLUS) $(GCC)
TARGET=dego
CPP_FILES=$(wildcard *.cpp)
OBJ_FILES=$(addprefix ./,$(notdir $(CPP_FILES:.cpp=.o)))

default: $(TARGET).iso

play: $(TARGET).iso
	qemu-system-i386 -cdrom $(TARGET).iso

clean_compiler: clean
	rm -rf compiler/gcc*
	rm -rf compiler/binutils*
	rm -rf compiler/bin

$(INSTALL_PATH)/bin/bin/$(AS):
	/bin/bash -c "cd $(INSTALL_PATH) && make bin/bin/$(AS)"
$(INSTALL_PATH)/bin/bin/$(GCC): $(INSTALL_PATH)/bin/bin/$(AS)
	/bin/bash -c "cd $(INSTALL_PATH) && make bin/bin/$(GCC)"

compiler_built: $(INSTALL_PATH)/bin/bin/$(AS) $(INSTALL_PATH)/bin/bin/$(GCC)
	touch compiler_built

boot.o: boot.s compiler_built
	$(AS) boot.s -o boot.o

%.o: %.cpp
	$(GPLUS) -ffreestanding -O2 -Wall -Wextra -c -o $@ $^ 

$(TARGET).bin: boot.o kernel.o $(OBJ_FILES)
	$(GCC) -T linker.ld -o $(TARGET).bin -ffreestanding -O2 -nostdlib boot.o $(OBJ_FILES)

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
