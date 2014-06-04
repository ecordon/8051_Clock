// lcdlib.h
// Edwin Cordon

#ifndef LCDLIB_H
#define LCDLIB_H

// function declarations
void LCD_Init(void);
void IRWrite(char code);
void functionSet(void);
void sendString(char* str);
void sendChar(char c);
void delay(unsigned int time);

// define the command code 
#define DEC_CURSOR 4
#define INC_CURSOR   6
#define SHIFT_DISPLAY_RIGHT  5

// preprocessor macros
#define clearDisplay() IRWrite(1)
#define returnHome() IRWrite(2)
#define entryModeSet(id, s) IRWrite(0x04 | ((id) << 1) | (s))
#define displayOnOffControl(display, cursor, blinking) \
IRWrite(0x08 | ((display)<<2) | ((cursor)<<1) | (blinking))
#define setDdRamAddress(addr) IRWrite(0x80 | (addr))
#define cursorGoTo(row, col) \
setDdRamAddress(((row)<<6 & 0x40) + col)

#endif