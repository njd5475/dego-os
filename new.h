/*
 * new
 *
 *  Created on: Aug 1, 2015
 */

#ifndef NEW_H_
#define NEW_H_

#include <stddef.h>
#include "malloc.h"

void * operator new(size_t size);

void * operator new[](size_t size);

void operator delete(void *p);

void operator delete[](void *p);

void operator delete(void*, long unsigned int);

void operator delete[](void*, long unsigned int);

#endif /* NEW_H_ */
