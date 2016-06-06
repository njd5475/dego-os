#include "string_functions.h"

size_t strlen(const char* str) {
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}
void strcpy(char *dst, const char *src) {
	size_t ret = 0;
	while(src[ret] != 0) {
		dst[ret++] = src[ret];
	}
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
