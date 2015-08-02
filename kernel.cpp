#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "new.h"

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
enum vga_color {
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

unsigned char make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, unsigned char color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t strlen(const char* str) {
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}
uint32_t digitLength(uint32_t n, uint16_t base) {
	uint32_t i = 0;
	while (n > 0) {
		n /= base;
		++i;
	}
	return i;
}
uint32_t digitLength10(size_t n) {
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
				return (n < 10000) ? 4 : 5;
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

public:
	Condition() {}
	~Condition() {}
	bool check() {return false;}
};

struct Event {
	Condition preCondition;
	Action action;
};

class KernelBuilder {
public:
	KernelBuilder() {
	}

	KernelBuilder *when() {
		return this;
	}
	KernelBuilder *kernelStarts() {
		return this;
	}
	KernelBuilder *_do() {
		return this;
	}
	KernelBuilder *initializeTerminal() {
		return this;
	}
	KernelBuilder *putWord(const char *c, unsigned short index) {
		long unsigned int len = strlen(c);
		for (unsigned short i = 0; i < len; ++i) {
			t.putChar(c[i], index + i);
		}
		return this;
	}
	KernelBuilder *putNumber(unsigned int num, unsigned int index) {
		this->putNumber(num, index, 10);
		return this;
	}
	KernelBuilder *putNumber(unsigned int num, unsigned int index, unsigned char base) {
		unsigned int i = index
				+ ((base == 10) ? digitLength10(num) : digitLength(num, base));
		while (num > 0) {
			putChar(
					((num % base) > 10) ?
							('A' + ((num % base) - 10)) : ('0' + (num % base)),
					i);
			num /= base;
			--i;
		}
		return this;
	}
	KernelBuilder *drawRect(unsigned char row, unsigned char col, unsigned char width,
			unsigned char height) {
		t.drawRect(row, col, width, height);
		return this;
	}
	KernelBuilder *putChar(char c, unsigned short index) {
		t.putChar(c, index);
		return this;
	}
	KernelBuilder *putInt(unsigned short num) {
		unsigned short i = 0;
		unsigned short cpynum = num;
		while (cpynum > 0) {
			++i;
			cpynum /= 10;
		}
		while (num > 0) {
			unsigned short c = num % 10;
			char cc = (char) c;
			t.putChar(cc + ((char) '0'), --i);
			num /= 10;
		}
		return this;
	}
private:
	Terminal t;
};

/* Input a byte from a port */
inline unsigned char inportb(unsigned int port) {
	unsigned char ret;
	asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
	return ret;
}

/* Output a byte to a port */
/* July 6, 2001 added space between :: to make code compatible with gpp */

inline void outportb(unsigned int port, unsigned char value) {
	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

/* Stop Interrupts */
inline void stopints() {
	asm ("cli");
}

unsigned char highmem, lowmem;
uint16_t mem;

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	KernelBuilder b;

	// when kernel starts do actions
	b.putWord("Hello World! - Dego", 0);

	b.drawRect(25 - 5 - 1, 80 - 30 - 1, 30, 5);

	Event *type = new Event[30];
	for(int i = 0; i < 30; ++i) {
		type[i].action._do();
		type[i].preCondition.check();
	}

	b.putNumber((unsigned int)&endkernel, 80*6, 16);
	b.putNumber(lastAllocation, 80 * 5, 10);
	b.putNumber(sizeof(size_t), 80 * 7, 10);
//	char h[80];
//	h[0] = 'H';
//	unsigned int i = 0;
//	while (true) {
//		unsigned char k = inportb(0x64);
//		b.putNumber(k, 81, 16);
//		if (k == 0x1D) {
//			unsigned char p = inportb(0x60);
//			switch (p) {
//			case 0x9E:
//				h[++i] = 'A';
//			default:
//				h[++i] = ' ';
//			}
//			h[i + 1] = '\0';
//		}
//		b.putWord(h, 50);
//	}

	void *ptr = &endkernel;

	//searching memory
	for(unsigned int i = 0; i < 0xFFFF; ++i) {
		ptr += 1;
		*((unsigned int*)ptr) = 0;
		b.putNumber(*((unsigned int*)ptr), 80*8, 16);
		b.putNumber((unsigned int)ptr, 80*9, 16);
	}

	b.putNumber((unsigned int)(&endkernel-(unsigned int)ptr), 80*10, 10);
}

unsigned int calcIndex(unsigned int row, unsigned int col, unsigned int totalCol) {
	return row * totalCol + col;
}
