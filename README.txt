README
Edwin Cordon

To compile the program
-----------------------

% sdcc -c clock.c
% sdcc -c lcdlib.c
% sdcc --iram-size 0x100 --code-size 0x1000 --code-loc 0x0000 --stack-loc 0x30 --data-loc 0x30 clock.rel lcdlib.rel keylib.rel
% packihx clock.ihx > clock.hex

hex file is also included!


Using the clock
----------------

The clock has two modes of operation, clock mode and set mode, switch to set mode by holding the sw0 switch, and let go
to return to clock mode.

This clock works a little differently than specified in the assignment. Instead of using the '*' and '#' keys in the keypad
to move the cursor, the SW3 switch is used to move the cursor to the left, and sw4 is used to move it to the right. Pressing
the '*' or '#' will do nothing. 

When a key is hit in the keypad, the cursor will remain in the same place, this is to prevent having to move the cursor 
again if a key was hit by mistake. Once the user is sure of his decision, he can move the cursor using the sw3 or sw2 switches.

To switch between row, simply toggle sw1 and the cursor will move up or down. 

This clock displays the date as mm-dd-yy.