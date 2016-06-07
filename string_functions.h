
#ifndef STR_FUNCS_H_
#define STR_FUNCS_H_ 1

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);

void strcpy(char *dst, const char *src);

bool strcmp(const char *lval, const char *rval);

uint32_t digitLength(uint32_t n, uint16_t base);

uint32_t digitLength10(size_t n);

#endif
