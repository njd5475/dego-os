#include "kernel_builder.h"
#include "terminal.h"

KernelBuilder::KernelBuilder() : t(new Terminal()) {
}

KernelBuilder *KernelBuilder::when() {
  return this;
}

KernelBuilder *KernelBuilder::kernelStarts() {
  return this;
}

KernelBuilder *KernelBuilder::_do() {
  return this;
}

Terminal *KernelBuilder::terminal() {
  return t;
}

KernelBuilder *KernelBuilder::initializeTerminal() {
  return this;
}

KernelBuilder *KernelBuilder::putWord(const char *c, unsigned short row, unsigned short col) {
  t->putWord(c, calcIndex(row, col, COLUMNS));
  return this;
}

KernelBuilder *KernelBuilder::putCenteredWord(const char *c, unsigned short row) {
  t->putCenteredWord(c, row);
  return this;
}

KernelBuilder *KernelBuilder::putNumber(unsigned int num, unsigned short row, unsigned short col) {
  this->putNumber(num, calcIndex(row, col, COLUMNS), 10);
  return this;
}

KernelBuilder *KernelBuilder::putNumber(unsigned int num, unsigned int index, unsigned char base) {
  unsigned int i = index
      + ((base == 10) ? digitLength10(num) : digitLength(num, base));
  while (num > 0) {
    putChar(
        ((num % base) > 10) ?
            ('A' + ((num % base) - 10)) : ('0' + (num % base)),
        i);
    num /= base;
    --i;
  }
  return this;
}

KernelBuilder *KernelBuilder::drawRect(unsigned char row, unsigned char col, unsigned char width,
    unsigned char height) {
  t->drawRect(row, col, width, height);
  return this;
}

KernelBuilder *KernelBuilder::drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow) {
  t->drawCenteredRectAtRow(rows, cols, atRow);
}

KernelBuilder *KernelBuilder::putChar(char c, unsigned short index) {
  t->putChar(c, index);
  return this;
}

KernelBuilder *KernelBuilder::putInt(unsigned short num) {
  unsigned short i = 0;
  unsigned short cpynum = num;
  while (cpynum > 0) {
    ++i;
    cpynum /= 10;
  }
  while (num > 0) {
    unsigned short c = num % 10;
    char cc = (char) c;
    t->putChar(cc + ((char) '0'), --i);
    num /= 10;
  }
  return this;
}

KernelBuilder *KernelBuilder::put(const char* line) {
  t->printLine(line);
}
