#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "new.h"
#include "function.h"
#include "string_functions.h"
#include "script.h"
#include "kernel_builder.h"

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

extern char _binary_program_wh_start;
extern char _binary_program_wh_end;

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main() {
  KernelBuilder b;

  // when kernel starts do actions
  b.putWord("Hello World! - Dego", 3, 0);
  b.putCenteredWord("DegoOS-CC", 2);

  const char* p = &_binary_program_wh_start;
  Token *head = new Token(*p++);
  Token *next = head;

  int token_count = 0;
  while(*p) {
    Token *n = next->add(*p++);
    if(n != next) {
      ++token_count;
    }
    next = n;
  }

  b.putWord("Building AST...", 0, 0);
  Node *ast = NULL;
  if(ast == NULL) {
    b.putWord("BAD AST!", 0, 0);
  }else{
    //ast->print(&b);
    b.putWord("Still Good", 5, 0);
  }
  b.putWord("Done!", 0, 0);

  void *ptr = &endkernel;
}
