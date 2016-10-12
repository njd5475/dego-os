/*
 * function.h
 *
 *  Created on: Aug 2, 2015
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

/* Input a byte from a port */
unsigned char inportb(unsigned int port);

/* Output a byte to a port */
void outportb(unsigned int port, unsigned char value);

/* Stop Interrupts */
void stopints();

bool is_real();

#endif /* FUNCTION_H_ */
