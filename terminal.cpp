#include "terminal.h"

/* Hardware text mode color constants. */
enum vga_color {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15,
};

unsigned char make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t make_vgaentry(char c, unsigned char color) {
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
}

Terminal::Terminal() : buffer((uint16_t*) 0xB8000), rows(25), columns(COLUMNS), total(COLUMNS*25), color(0) {
  for(size_t i = 0; i < total; ++i) {
    buffer[i] = 0x0F00;
  }
}

void Terminal::drawRect(unsigned char row, unsigned char col, unsigned char width, unsigned char height) {
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

void Terminal::putChar(uint16_t c, size_t row, size_t col) {
  putChar(c, calcIndex(row, col));
}

void Terminal::putWord(const char *c, size_t row, size_t col) {
  putWord(c, calcIndex(row, col));
}

void Terminal::putChar(uint16_t c, size_t index) {
  buffer[index] = buffer[index] & 0xFF00; //clear bottom
  buffer[index] = buffer[index] | c;
}

const char Terminal::get(size_t row, size_t col) {
  return get(calcIndex(row, col));
}

const char Terminal::get(size_t index) {
  return (const char)((buffer[index] & 0x00FF));
}

void Terminal::clearChar(size_t row, size_t col) {
  putChar(0, row, col);
}

void Terminal::clearChar(size_t index) {
  putChar(0, index);
}

void Terminal::putWord(const char *c, unsigned short index) {
  long unsigned int len = strlen(c);
  for (unsigned short i = 0; i < len; ++i) {
    putChar(c[i], index + i);
  }
}

void Terminal::putCenteredWord(const char *c, unsigned short row) {
  unsigned int start_index = (columns /2) - (strlen(c) / 2);
  putWord(c, start_index);
}

void Terminal::drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow) {
  drawRect(atRow, (columns /2) - (cols/2), cols, rows);
}

void Terminal::printLine(const char* line) {
  //start at line zero and move each line up one in the buffer
  bool lineEnded = false;
  for(size_t row = 1; row <= rows; ++row) {
    for(size_t col = 0; col < columns; ++col) {
      if(row == rows) {
        if(!lineEnded) {
          lineEnded = (line[col] == 0);
        }
        putChar(lineEnded ? ' ' : line[col], row-1, col);
      }else{
        putChar(get(row, col), row-1, col);
      }
    }
  }
}

void Terminal::print(const char* line) {
}
