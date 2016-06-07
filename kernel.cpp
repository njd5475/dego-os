#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "new.h"
#include "function.h"
#include "string_functions.h"
#include "script.h"

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "Use a cross-compiler, please "
#endif

/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

#include "context.h"
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

unsigned int calcIndex(unsigned int row, unsigned int col, unsigned int totalCol) {
  return row * totalCol + col;
}

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
  KernelBuilder *putWord(const char *c, unsigned short row, unsigned short col) {
    t.putWord(c, calcIndex(row, col, COLUMNS));
    return this;
  }
  KernelBuilder *putCenteredWord(const char *c, unsigned short row) {
    t.putCenteredWord(c, row);
    return this;
  }
  KernelBuilder *putNumber(unsigned int num, unsigned short row, unsigned short col) {
    this->putNumber(num, calcIndex(row, col, COLUMNS), 10);
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
  KernelBuilder *drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow) {
    t.drawCenteredRectAtRow(rows, cols, atRow);
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

extern char _binary_program_wh_start;
extern char _binary_program_wh_end;

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main() {
  KernelBuilder b;

  // when kernel starts do actions
  b.putWord("Hello World! - Dego", 0);
  b.putCenteredWord("DegoOS-CC", 2);

  const char* p = &_binary_program_wh_start;
  Token *next = new Token(*p++);

  int token_count = 0;
  while(*p) {
    Token *n = next->add(*p++);
    if(n != next) {
      ++token_count;
    }
    next = n;
  }

  //b.putNumber(token_count, 0);
//  char h[80];
//  h[0] = 'H';
//  unsigned int i = 0;
//  while (true) {
//    unsigned char k = inportb(0x64);
//    b.putNumber(k, 81, 16);
//    if (k == 0x1D) {
//      unsigned char p = inportb(0x60);
//      switch (p) {
//      case 0x9E:
//        h[++i] = 'A';
//      default:
//        h[++i] = ' ';
//      }
//      h[i + 1] = '\0';
//    }
//    b.putWord(h, 50);
//  }

  void *ptr = &endkernel;

}

unsigned int calcIndex(unsigned int row, unsigned int col, unsigned int totalCol) {
  return row * totalCol + col;
}
