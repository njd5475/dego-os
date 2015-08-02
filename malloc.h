/*
 * malloc.h
 *
 *  Created on: Aug 1, 2015
 *      Author: pbywebapp
 */

#ifndef MALLOC_H_
#define MALLOC_H_


extern uint32_t endkernel;
void *location = &endkernel;
size_t lastAllocation;
void *malloc(size_t size) {
	void *newLoc = location;
	location += size;
	lastAllocation = size;
	return newLoc;
}

void free(void *) {

}

#endif /* MALLOC_H_ */
