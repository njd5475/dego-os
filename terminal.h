
#ifndef TERMINAL_H
#define TERMINAL_H 1

#include "kernel.h"
#include "string_functions.h"

class Terminal
{
public:
	Terminal();

	const char get(size_t row, size_t col);
	const char get(size_t index);
	void drawRect(unsigned char row, unsigned char col, unsigned char width, unsigned char height);
	void clearChar(size_t row, size_t col);
	void clearChar(size_t index);
	void putChar(uint16_t c, size_t row, size_t col);
	void putWord(const char *c, size_t row, size_t col);
	void putChar(uint16_t c, size_t index);
	void putWord(const char *c, unsigned short index);
	void putCenteredWord(const char *c, unsigned short row);
	void drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow);
	void printLine(const char *c);
	void print(const char *c);

protected:
	unsigned int calcIndex(unsigned int row, unsigned int col) {
	  return row * columns + col;
	}

private:
	uint16_t* buffer;
	size_t curLine;
	const unsigned char columns;
	const unsigned char rows;
	const size_t total;
	unsigned char color;
};

#endif
