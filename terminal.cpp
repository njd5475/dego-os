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

#define BCLEAR 0x0F00

unsigned char make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t make_vgaentry(char c, unsigned char color) {
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
}

Terminal::Terminal() : buffer((uint16_t*) 0xB8000), rows(25), columns(COLUMNS), totalChars(COLUMNS*25), color(0), curCol(0), curLine(0) {
  for(size_t i = 0; i < totalChars; ++i) {
    buffer[i] = BCLEAR;
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
  this->putChar(c, this->calcIndex(row, col));
}

void Terminal::putChar(uint16_t c, size_t index) {
  this->buffer[index] = this->buffer[index] & 0xFF00; //clear bottom
  this->buffer[index] = this->buffer[index] | c;
}

const char Terminal::get(size_t row, size_t col) {
  return this->get(this->calcIndex(row, col));
}

const char Terminal::get(size_t index) {
  return (const char)((this->buffer[index] & 0x00FF));
}

void Terminal::clearChar(size_t row, size_t col) {
  this->clearChar(0, this->calcIndex(row, col));
}

void Terminal::clearChar(size_t index) {
  this->buffer[index] &= BCLEAR; // clear both
}

void Terminal::putWord(const char *c, size_t row, size_t col) {
  this->putWord(c, this->calcIndex(row, col));
}

void Terminal::putWord(const char *c, unsigned short index) {
  long unsigned int len = strlen(c);
  for (unsigned short i = 0; i < len; ++i) {
    this->putChar(c[i], index + i);
  }
}

void Terminal::putCenteredWord(const char *c, unsigned short row) {
  unsigned int start_index = (columns /2) - (strlen(c) / 2);
  this->putWord(c, start_index);
}

void Terminal::drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow) {
  this->drawRect(atRow, (columns /2) - (cols/2), cols, rows);
}

void Terminal::lineFeed() {
  if(this->curLine < this->rows) {
    this->curCol = 0;
    ++this->curLine;
  }else{
    for(int row = 1; row < this->rows-1; ++row) {
      for(int col = 0; col < this->columns; ++col) {
        this->putChar(this->get(row, col), row-1, col);
      }
    }
  }
}

void Terminal::printLine(const char* line) {
  //this->lineFeed();

  // now do the last row
  if(this->curCol > 0) {
    ++this->curLine;
    this->curCol = 0;
  }
  this->print(line);

  ++this->curLine;
  this->curCol = 0;

  if(this->curLine >= this->rows) {
    this->curLine = this->rows;
  }
}

void Terminal::print(const char* line) {
  int li = 0;
  while(line[li] != 0 && li < this->columns) {
    this->putChar(line[li], this->curLine, this->curCol);
    ++li;
    ++this->curCol;
  }

  if(this->curCol == columns) {
    this->curCol = 0;
    ++this->curLine;

    if(this->curLine >= this->rows) {
      this->curLine = this->rows;
    }
  }
}
