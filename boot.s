# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard. Section so we can force the 
# header to be in the start of the final program.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# set a necessary small temporary stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# start location for linker script and location bootloader will jump to.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp #setup stack, moves downwards

	call kernel_main #call into c++ kernel

	cli # clear interrupt flag
	hlt # tell the processor loop infinitely we're not doing anything
.Lhang:
	jmp .Lhang # just in-case we'll do an infinite loop

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start
