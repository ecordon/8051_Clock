#ifndef __KEYLIB_H__
#define __KEYLIB_H__

char getChar(void); // get key ascii, nonblocking
char waitKey(void); // get key ascii, blocking till key up
char getKey(void); // get key in index

#endif // __KEYLIB_H__