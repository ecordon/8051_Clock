// lcdlib.c
// Edwin Cordon

#include <8051.h>
#include "lcdlib.h"

#define DB7 P1_7
#define DB6 P1_6
#define DB5 P1_5
#define DB4 P1_4
#define DB  P1
#define RS P1_3
#define E P1_2

void LCD_Init(void) {
	functionSet();
	entryModeSet(1, 1); // increment and no shift
}

void IRWrite(char c) {

	RS = 0;	//clear RS (indicates that instructions are being sent to LCD module)
	DB7 = (c>>7)&1;
	DB6 = (c>>6)&1;
	DB5 = (c>>5)&1;
	DB4 = (c>>4)&1;
	E = 1;
	E = 0;
	DB7 = (c>>3)&1;
	DB6 = (c>>2)&1;
	DB5 = (c>>1)&1;
	DB4 = c&1;
	E = 1;
	E = 0;
	delay(50);
}

void functionSet(void) {
		
	RS = 0;	
	DB7 = 0;
	DB6 = 0;
	DB5 = 1;
    DB4 = 0;
	E = 1;
	E = 0;
    delay(50);
	E = 1;
	E = 0;
	DB7 = 1;
	E = 1;
	E = 0;
	delay(50);
}

void sendChar(char c) {
		
	DB7 = (c>>7)&1;
	DB6 = (c>>6)&1;
	DB5 = (c>>5)&1;
	DB4 = (c>>4)&1;
	RS = 1;
	E = 1;
	E = 0;
	DB7 = (c>>3)&1;
	DB6 = (c>>2)&1;
	DB5 = (c>>1)&1;
	DB4 = c&1;
	E = 1;
	E = 0;
	delay(50);
}

void sendString(char* str) {
	char c;
	while (c = *str++) {
		sendChar(c);
	}
}

void delay(unsigned int time){

	unsigned int i;
	for(i = 0; i < time; i++);
}
