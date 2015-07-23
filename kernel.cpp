#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "Use a cross-compiler, please "
#endif
 
/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

#include "action.h"
#include "terminal.h"
 
/* Hardware text mode color constants. */
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 
uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
uint16_t digitLength(uint16_t n, uint16_t base)  
{
	uint16_t i = 0;
	while(n > 0) {
		n /= base;
		++i;
	}
	return i;
}
uint16_t digitLength10(size_t n)
{
	if (n < 100000) {
		// 5 or less
		if (n < 100) {
			// 1 or 2
			return (n < 10) ? 1 : 2;
		} else {
			// 3 or 4 or 5
			if (n < 1000)
				return 3;
			else {
				// 4 or 5
				return (n < 10000) ?  4 : 5;
			}
		}
	} else {
		// 6 or more
		if (n < 10000000) {
			// 6 or 7
			return (n < 1000000) ? 6 : 7;
		 } else {
			// 8 to 10
			return (n < 100000000) ? 8 : ((n < 1000000000) ? 9 : 10);
		}
	}
}

class Condition {

};

struct Event {
	Condition preCondition;
	Action action;
};

class KernelBuilder
{
public:
	KernelBuilder() {}

	KernelBuilder *when() { return this; }
	KernelBuilder *kernelStarts() { return this; }
	KernelBuilder *_do() { return this; }
	KernelBuilder *initializeTerminal() { 
		return this;
	}
	KernelBuilder *putWord(const char *c, uint16_t index) {
		size_t len = strlen(c);
		for(uint16_t i = 0; i < len; ++i) {
			t.putChar(c[i], index+i);
		}
		return this;
	}
	KernelBuilder *putNumber(uint16_t num, uint16_t index) {
		this->putNumber(num,index,10);
		return this;
	}
        KernelBuilder *putNumber(uint16_t num, uint16_t index, uint8_t base) {
		uint16_t i = index + ((base == 10) ? digitLength10(num) : digitLength(num,base));
		while(num > 0) {
			putChar(((num % base) > 10) ? ('A' + ((num % base)-10)) : ('0' + (num % base)), i);
			num /= base;
			--i;
		}
		return this;
        }
	KernelBuilder *drawRect(uint8_t row, uint8_t col, uint8_t width, uint8_t height) {
		t.drawRect(row, col, width, height);
		return this;
	}
        KernelBuilder *putChar(char c, uint16_t index) {
		t.putChar(c, index);
		return this;
	}
	KernelBuilder *putInt(uint16_t num) {
		uint16_t i = 0;
		uint16_t cpynum = num;
		while(cpynum > 0) {
			++i;
			cpynum /= 10;
		}
		while(num > 0) {
			uint16_t c = num % 10;
			char cc = (char)c;
			t.putChar(cc + ((char)'0'), --i);
			num /= 10;
		}
		return this;
	}
private:
	Terminal t;
};

/* Input a byte from a port */

inline unsigned char inportb(unsigned int port)
{
   unsigned char ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}


/* Output a byte to a port */
/* July 6, 2001 added space between :: to make code compatible with gpp */

inline void outportb(unsigned int port,unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}


/* Stop Interrupts */
inline void stopints()
{
  asm ("cli");
}

unsigned char highmem, lowmem;
uint16_t mem;

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
	KernelBuilder b;

	// when kernel starts do actions
	b.putWord("Hello World! - Dego", 0);

	b.drawRect(25-5-1, 80-30-1, 30, 5);
	
	b.putNumber(inportb(0x64), 81, 2);
}
