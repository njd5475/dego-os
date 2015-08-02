/*
 * protected.c
 *
 *  Created on: Aug 2, 2015
 */

#include "functions.h"

void NMI_enable(void) {
	outportb(0x70, inb(0x70) & 0x7F);
}

void NMI_disable(void) {
	outportb(0x70, inb(0x70) | 0x80);
}

