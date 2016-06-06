
#ifndef TERMINAL_H
#define TERMINAL_H 1

#include "string_functions.h"

class Terminal
{
public:
	Terminal() : buffer((uint16_t*) 0xB8000), rows(25), columns(80), total(80*25), color(0) {
		for(size_t i = 0; i < total; ++i) {
			buffer[i] = 0x0F00;
		}
	}

	void drawRect(unsigned char row, unsigned char col, unsigned char width, unsigned char height) {
		#define TOP_LEFT  218
		#define BOT_RIGHT 217
		#define BOT_LEFT  192
		#define TOP_RIGHT 191
		#define TOP_BOT	196	
		#define LEFT_RIGHT 179	
		putChar(TOP_LEFT, row * columns + col);	
		putChar(BOT_LEFT, (row+height) * columns + col);
		putChar(BOT_RIGHT, (row+height) * columns + (col+width));	
		putChar(TOP_RIGHT, row * columns + (col+width));	

		for(unsigned char i = row+1; i < (row+height); ++i) {
			putChar(LEFT_RIGHT, i * columns + col);
			putChar(LEFT_RIGHT, i * columns + (col+width));
		}
		for(unsigned char i = col+1; i < (col+width); ++i) {
			putChar(TOP_BOT, row * columns + i);
			putChar(TOP_BOT, (row+height) * columns + i);
		}
	}

	void putChar(uint16_t c, size_t index) {
		buffer[index] = buffer[index] & 0xFF00; //clear bottom
		buffer[index] = buffer[index] | c;
	}
	void putWord(const char *c, unsigned short index) {
		long unsigned int len = strlen(c);
		for (unsigned short i = 0; i < len; ++i) {
			putChar(c[i], index + i);
		}
	}
	void putCenteredWord(const char *c, unsigned short row) {
		unsigned int start_index = (columns /2) - (strlen(c) / 2);
		putWord(c, start_index);
	}
	void drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow) {
		drawRect(atRow, (columns /2) - (cols/2), cols, rows);
	}


private:
	uint16_t* buffer;
	const unsigned char columns;
	const unsigned char rows;
	const size_t total;
	unsigned char color;
};

#endif
