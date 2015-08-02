/*
 * new
 *
 *  Created on: Aug 1, 2015
 *      Author: pbywebapp
 */

#ifndef NEW_H_
#define NEW_H_

#include <stddef.h>
#include "malloc.h"

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}

#endif /* NEW_H_ */
