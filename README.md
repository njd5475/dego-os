# dego-os
An Operating System

Requirements
===

* git - revision control
* gcc -  due to issues building gcc without an installed package
* Grub-rescue - for iso and boot wrapper.
* Xorriso - needed by grub
* Qemu - for emulation

Build scripts have been test on Ubuntu 14.04 and Debian 8.2 built using an elf cross compiler. 

Dependency Versions
===

Build was working as of 2015-11-17 using the following versions of dependencies:

* System gcc (Debian 4.9.2-10) 4.9.2
* Cross compiler built using:
	* gcc 5.2.9
		* gmp 4.3.2
		* isl 0.14
		* mpc 0.8.1
		* mpfr 2.4.2
	* binutils 2.25
* QEMU emulator version 2.1.2 (Debian 1:2.1+dfsg-12+deb8u4)
* grub-mkrescue (GRUB) 2.02~beta2-22
	* xorriso libisoburn in use :  1.3.2  (min. 1.3.2)

Building
===

Install pre-requisite packages on a linux system.
	
		`$ sudo apt-get install git build-essential qemu xorriso`

Clone the repo and from the commandline run

		`$ make play`

WARNING: first run will build the cross-compiler which will take at least an hour on a single core.
