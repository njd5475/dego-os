#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
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
uint16_t digitLength(size_t n)
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

 
class Terminal
{
public:
	Terminal() : buffer((uint16_t*) 0xB8000), columns(80), total(80*25), color(0) {
		for(size_t i = 0; i < total; ++i) {
			buffer[i] = 0x0F00;
		}
	}

	void drawRect(uint8_t row, uint8_t col, uint8_t width, uint8_t height) {
		#define TOP_LEFT  218
		#define BOT_RIGHT 217
		#define BOT_LEFT  192
		#define TOP_RIGHT 191
		#define TOP_BOT	196	
		#define LEFT_RIGHT 179	
		putChar(TOP_LEFT, row * columns + col);	
		putChar(BOT_LEFT, (row+height) * columns + col);
		putChar(BOT_RIGHT, (row+height) * columns + (col+width));	
		putChar(TOP_RIGHT, row * columns + (col+width));	

		for(uint8_t i = row+1; i < (row+height); ++i) {
			putChar(LEFT_RIGHT, i * columns + col);
			putChar(LEFT_RIGHT, i * columns + (col+width));
		}
		for(uint8_t i = col+1; i < (col+width); ++i) {
			putChar(TOP_BOT, row * columns + i);
			putChar(TOP_BOT, (row+height) * columns + i);
		}
	}

	void putChar(uint16_t c, size_t index) {
		buffer[index] = buffer[index] & 0xFF00; //clear bottom 		
		buffer[index] = buffer[index] | c;
	}


private:
	uint16_t* buffer;
	uint8_t columns;
	size_t total;
	uint8_t color;
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
	KernelBuilder *putWord(char *c, uint16_t index) {
		size_t len = strlen(c);
		for(uint16_t i = 0; i < len; ++i) {
			t.putChar(c[i], index+i);
		}
	}
        KernelBuilder *putNumber(uint16_t num, uint16_t index) {
		uint16_t i = index + digitLength(num);
		while(num > 0) {
			putChar('0' + (num % 10), i);
			num /= 10;
			--i;
		}
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
	b.when()->kernelStarts()->_do()->putWord("Hello World! - Dego", 0);

	uint16_t i = 0;
	while(true) {
		b.when()->kernelStarts()->_do()->putNumber(i, 81);
		++i;
	}

	/* Since there is no support for newlines in terminal_putchar yet, \n will
	   produce some VGA specific character instead. This is normal. */

/* need to stop ints before accessing the CMOS chip */
  stopints();

/* write to port 0x70 with the CMOS register we want to read */
/* 0x30 is the CMOS reg that hold the low byte of the mem count */
  outportb(0x70,0x30);


/* read CMOS values from port 0x71 */
  lowmem = inportb(0x71);


/* write to port 0x70 with the CMOS register we want to read */
/* 0x31 is the CMOS reg that hold the high byte of the mem count */
  outportb(0x70,0x31);


/* read CMOS values from port 0x71 */
  highmem = inportb(0x71);


/* fix the low and high bytes into one value */
  mem = highmem;
  mem = mem<<8;
  mem += lowmem;

  //b.putInt(mem);
}
