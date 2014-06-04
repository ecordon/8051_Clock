// clock.c 
// Edwin Cordon 
/*
	Clock and calendar program for 8051 microcontroller
*/

#include <8051.h>
#include "lcdlib.h"
#include "keylib.h"

char timeStr[] = "00:00:00";
char dateStr[] = "00-00-00";
char dispStr1[] = "        ";
char dispStr2[] = "        ";

#define SW0 P2_0	// pin for switching between operateMode and setMode	
#define SW1 P2_1	// pin for switching the cursor between lines in the LCD
#define SW2 P2_2	// pin for shifting cursor in the LCD to the right
#define SW3 P2_3	// pin for shifting cursor in the LCD to the left

unsigned int days_in_month[] = {31, 28, 31, 30, 30, 30, 31, 31, 30, 31, 30, 31};

int hour = 0, minute = 0, second = 0; 
int day = 1, month = 1, year = 0;
char c;				// character read from keypad
int col_cursor = 0, row_cursor = 0;

// clock mode functions
void clockMode(void);
void updateTime(void);
void updateDate(void);
void updateTimeString(void);
void updateDateString(void);
void refreshLCD(void);

// set mode functions
void setMode();
void navigateCursor(void);
void moveCursorRight(void);
void moveCursorLeft(void);
void translateTime(void);
void translateDate(void);
void validateTime(void);
void validateDate(void);
void setDigit(void);
void longDelay(void);

void Main(void) __naked {

	LCD_Init();

	while (1) {
		
		// initialize LCD for clock mode
		displayOnOffControl(1, 0, 0);	// display on, cursor off and blinking off 
		while(SW0 == 1){
			clockMode();
		}

		// reset cursors
		row_cursor = 0; col_cursor = 0;
		cursorGoTo(row_cursor, col_cursor);

		// initialize LCD for set mode
		displayOnOffControl(1, 1, 1);	// display on, cursor on and blinking on 
		while(SW0 == 0){
			setMode();
		}
	}
}

void clockMode(void) {
	updateTime();
	refreshLCD();
}

void updateTime(void) {

	second++;

	//modulo seconds by 60
	if(second >= 60){
		second = 0;
		minute++;
	}

	//modulo minutes by 60
	if(minute >= 60){
		minute = 0;
		hour++;
	}

	//mudulo hours by 24
	if(hour >= 24){
		hour = 0;
		updateDate();
	}

	updateTimeString();
}

void updateDate(){

	day++;

	//modulo days depending on month
	if(year%4 != 0 || month != 2){	//if year is not a leap year, or month is not February
		if(day >= (days_in_month[month-1] + 1)){
			day = 1;
			month++;
		}
	}
	else{
		if(day >= 30){	//reset day once it goes over 29 in February of a leap year
			day = 1;
			month++;
		}
	}

	//modulo months by 12
	if(month >= 13){
		month = 1;
		year++;
	}

	//modulo year by 100
	if(year >= 100){
		year = 0;
	}

	updateDateString();
}

void updateTimeString() {
	
	//write hour to timeStr
	timeStr[0] = hour/10 + 48;
	timeStr[1] = hour%10 + 48;
	
	//write minute to timeStr
	timeStr[3] = minute/10 + 48;
	timeStr[4] = minute%10 + 48;
	
	//write second to timeStr
	timeStr[6] = second/10 + 48;
	timeStr[7] = second%10 + 48;
}

void updateDateString() {

	//write month to dateStr
	dateStr[0] = month/10 + 48;
	dateStr[1] = month%10 + 48;
	
	//write day to dateStr
	dateStr[3] = day/10 + 48;
	dateStr[4] = day%10 + 48;
	
	//write year to dateStr
	dateStr[6] = year/10 + 48;
	dateStr[7] = year%10 + 48;
}

void refreshLCD() {

	unsigned int i;

	for(i = 0; i < 8; i++){
		
		//check digits for timeStr and refresh updated ones only
		if(timeStr[i] != dispStr1[i]){
			dispStr1[i] = timeStr[i];
			cursorGoTo(0, i);
			sendChar(dispStr1[i]);
		}
		
		//updates chars in dateStr
		if(dateStr[i] != dispStr2[i]){
			dispStr2[i] = dateStr[i];
			cursorGoTo(1, i);
			sendChar(dispStr2[i]);
		}
	}
}

void setMode(void) {
	navigateCursor();
	setDigit();
}

void navigateCursor(void) {

	// navigate cursor to the left
	if(SW3 == 0){
		longDelay();
		col_cursor--;
		if(col_cursor == 2 || col_cursor == 5){	
			col_cursor--;
		}
		if(col_cursor == -1){
			col_cursor = 7;		
		}
		cursorGoTo(row_cursor, col_cursor);
	}

	// navigate cursor to the right
	else if(SW2 == 0){
		longDelay();
		col_cursor = (col_cursor+1)%8;
		if(col_cursor == 2 || col_cursor == 5){	
			col_cursor++;
		}
		cursorGoTo(row_cursor, col_cursor);
	}

	// navigate cursor up and down
	else if(SW1 == 0){
		longDelay();
		row_cursor = (row_cursor+1)%2;
		cursorGoTo(row_cursor, col_cursor);
	}
}

void setDigit(void) {

	c = getChar();

	if(c && c != '*' && c != '#'){
		if(row_cursor == 0){
			translateTime();
			sendChar(c);
			cursorGoTo(row_cursor, col_cursor);
		}
		else{
			translateDate();
			sendChar(c);
			cursorGoTo(row_cursor, col_cursor);
		}
		
	}
}

/*
	NOTE: It's not a good idea to use seperate functions for translating time and date since	
	both functions work exactly the same way. However it is necessary in this case because SDCC
	complains when we try making a function that can handle both cases.
*/

void translateTime(void) {

	switch(col_cursor){

		case 0:{	// set first digit in hour
			hour = 10*(c - 48) + hour%10;
			break;
		}
		case 1:{	// set second digit in hour
			hour = 10*(hour/10) + (c - 48);
			break;
		}
		case 3:{	// set first digit in minute
			minute = 10*(c - 48) + minute%10;
			break;
		}
		case 4:{	// set second digit in minute
			minute = 10*(minute/10) + (c - 48);
			break;
		}
		case 6:{	// set first digit in second
			second = 10*(c - 48) + second%10;
			break;
		}
		case 7:{	// set second digit in second
			second = 10*(second/10) + (c - 48);
			break;
		}
		default:{
			break;
		}
	}
	validateTime();
	updateTimeString();
}

void translateDate() {

	switch(col_cursor){

		case 0:{	// set first digit in month
			month = 10*(c - 48) + month%10;
			break;
		}
		case 1:{	// set second digit in month
			month = 10*(month/10) + (c - 48);
			break;
		}
		case 3:{	// set first digit in day
			day = 10*(c - 48) + day%10;
			break;
		}
		case 4:{	// set second digit in day
			day = 10*(day/10) + (c - 48);
			break;
		}
		case 6:{	// set first digit in year
			year = 10*(c - 48) + year%10;
			break;
		}
		case 7:{	// set second digit in year
			year = 10*(year/10) + (c - 48);
			break;
		}
		default:{
			break;
		}
	}
	validateDate();
	updateDateString();
}

void validateTime() {

	if(second > 59){
		second = 59;
	}

	if(minute > 59){
		minute = 59;
	}

	if(hour > 23){
		hour = 23;
	}
}

void validateDate() {
	
	if(year%4 != 0 || month != 2){	//if year is not a leap year, or month is not February
		if(day > (days_in_month[month-1])){
			day = days_in_month[month-1];
		}
	}
	else{
		if(day > 29){	
			day = 29;
		}
	}

	if(month > 12){
		month = 12;
	}

	if(year > 99){
		year = 99;
	}
}

void longDelay(){
	char i;
	for(i = 0; i < 20; i++){
		delay(255);
	}
}

