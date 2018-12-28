TARGET=i686-elf
PREFIX = $(shell pwd)/bin
WGET=/usr/bin/wget
GCC_OPTS= --disable-nls --enable-languages=c,c++ --without-headers
GCC_VERSION=6.1.0


binutils-2.25.tar.gz:
	$(WGET) http://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz	
binutils-2.25: binutils-2.25.tar.gz
	/bin/bash -c "tar -zxf binutils-2.25.tar.gz" 
bin/bin/i686-elf-as: binutils-2.25
	echo $(REALPREFIX)
	/bin/bash -c "cd binutils-2.25 && ./configure --prefix=$(PREFIX) --target=$(TARGET) && make && make install"

gcc-$(GCC_VERSION).tar.bz2:
	$(WGET) ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.bz2
gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.bz2
	bash -c "tar -jxf gcc-$(GCC_VERSION).tar.bz2"
bin/bin/i686-elf-gcc: gcc-$(GCC_VERSION)
	/bin/bash -c "cd gcc-$(GCC_VERSION) && ./contrib/download_prerequisites"
	/bin/bash -c "cd gcc-$(GCC_VERSION) && ./configure --prefix=$(PREFIX) --target=$(TARGET) $(GCC_OPTS) && make -j8 all-gcc && make -j8 install-gcc"

all: bin/bin/i686-elf-as bin/bin/i686-elf-gcc
default: all
