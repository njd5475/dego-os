/*
 * malloc.h
 *
 *  Created on: Aug 1, 2015
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stddef.h>
#include <stdint.h>

extern uint32_t endkernel;

void *malloc(size_t size);

void free(void *ptr);

#endif /* MALLOC_H_ */
