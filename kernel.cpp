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
  b.put("Hello World! - Dego");
  if(is_real()) {
    b.put("We are in real mode");
  }else{
    b.put("We are in protected mode");
  }

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

  Parser parser(&b);
  Node *ast = parser.parse(head);
  if(ast == NULL) {
    b.put("BAD AST!");
  }else{
    b.put("Got ast printing...");
    ast->print(&b);
  }
  b.put("Done building programs!");

  void *ptr = &endkernel;
}
