#ifndef KERNEL_BUILDER_H_
#define KERNEL_BUILDER_H_

class Terminal;

class KernelBuilder {
public:
  KernelBuilder();

  KernelBuilder *when();
  KernelBuilder *kernelStarts();
  KernelBuilder *_do();
  Terminal *terminal();
  KernelBuilder *initializeTerminal();
  KernelBuilder *putWord(const char *c, unsigned short row, unsigned short col);
  KernelBuilder *putCenteredWord(const char *c, unsigned short row);
  KernelBuilder *putNumber(unsigned int num, unsigned short row, unsigned short col);
  KernelBuilder *putNumber(unsigned int num, unsigned int index, unsigned char base);

  KernelBuilder *drawRect(unsigned char row, unsigned char col, unsigned char width,
      unsigned char height);
  KernelBuilder *drawCenteredRectAtRow(unsigned short rows, unsigned short cols, unsigned short atRow);
  KernelBuilder *putChar(char c, unsigned short index);
  KernelBuilder *putInt(unsigned short num);

private:
  unsigned int calcIndex(unsigned int row, unsigned int col, unsigned int totalCol) {
    return row * totalCol + col;
  }

  Terminal *t;
};

#endif
