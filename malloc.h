/*
 * malloc.h
 *
 *  Created on: Aug 1, 2015
 *      Author: pbywebapp
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stddef.h>

extern uint32_t endkernel;
size_t *location = &endkernel;
size_t lastAllocation;

void *malloc(size_t size) {
	size_t *newLoc = location;
	location += size + sizeof(size_t);
	*newLoc = size;
	newLoc += sizeof(size_t);
	lastAllocation = size;
	return newLoc;
}

void free(void *ptr) {
	void *ptr2 = ptr - sizeof(size_t);
	size_t block_size = *((long unsigned int*)ptr2);
	*((unsigned int*)ptr) = 0xD1;
	ptr2 = ptr + block_size;
	*((unsigned int*)ptr2) = 0xD1 ^ 0xFF;
}

#endif /* MALLOC_H_ */
