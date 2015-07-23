
#ifndef TERMINAL_H
#define TERMINAL_H 1

class Terminal
{
public:
	Terminal() : buffer((uint16_t*) 0xB8000), columns(80), total(80*25), color(0) {
		for(size_t i = 0; i < total; ++i) {
			buffer[i] = 0x0F00;
		}
	}

	void drawRect(uint8_t row, uint8_t col, uint8_t width, uint8_t height) {
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

		for(uint8_t i = row+1; i < (row+height); ++i) {
			putChar(LEFT_RIGHT, i * columns + col);
			putChar(LEFT_RIGHT, i * columns + (col+width));
		}
		for(uint8_t i = col+1; i < (col+width); ++i) {
			putChar(TOP_BOT, row * columns + i);
			putChar(TOP_BOT, (row+height) * columns + i);
		}
	}

	void putChar(uint16_t c, size_t index) {
		buffer[index] = buffer[index] & 0xFF00; //clear bottom 		
		buffer[index] = buffer[index] | c;
	}


private:
	uint16_t* buffer;
	uint8_t columns;
	size_t total;
	uint8_t color;
};

#endif
