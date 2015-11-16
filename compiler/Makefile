TARGET=i686-elf
PREFIX = $(shell pwd)/bin
WGET=/usr/bin/wget

binutils-2.25.tar.gz:
	$(WGET) http://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.gz	
binutils-2.25: binutils-2.25.tar.gz
	/bin/bash -c "tar -zxf binutils-2.25.tar.gz" 
bin/bin/i686-elf-as: binutils-2.25
	echo $(REALPREFIX)
	/bin/bash -c "cd binutils-2.25 && ./configure --prefix=$(PREFIX) --target=$(TARGET) && make && make install"

gcc-5.2.0.tar.bz2:
	$(WGET) ftp://ftp.gnu.org/gnu/gcc/gcc-5.2.0/gcc-5.2.0.tar.bz2
gcc-5.2.0: gcc-5.2.0.tar.bz2
	bash -c "tar -jxf gcc-5.2.0.tar.bz2"
bin/bin/i686-elf-g++: gcc-5.2.0
	/bin/bash -c "cd gcc-5.2.0 && ./contrib/download_prerequisites"
	/bin/bash -c "cd gcc-5.2.0 && ./configure --prefix=$(PREFIX) --target=$(TARGET) && make -j8 all-gcc && make -j8 install-gcc"

all: bin/bin/i686-elf-as bin/bin/i686-elf-g++
default: all