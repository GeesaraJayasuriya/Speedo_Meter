# Speedo_Meter
The coursework for this module is a single project, ‘Firmware design for a DC motor speed  controller’. This is an individual project, with a short demonstration of the final firmware you  will produce
<<<<<<< HEAD
=======


1.4 Features and specification
Your firmware must implement the following features:
1 Display (Main Operation)
1.1 The 2x16 LCD display should display two three-digit values, along with 
appropriate text, to indicate the demanded speed (what is set by the operator 
via the keypad) and the actual speed (as measured). The manner and position 
on the display of these two values and accompanying text is up to the 
designer, but at all times when the system is powered, the display should be 
legible and there should be no ambiguity between the values as displayed. 
Such a display could look like:
DEMAND SPD: 200
ACTUAL SPD: 200
1.2 Leading zeroes should not be suppressed.
1.3 When the operator changes the speed via the keypad, a cursor on the 
display over the demanded speed should allow the operator to visualize the 
entry of one digit at a time. This will include the backspace function (see4).
1.4 If the operator enters a speed outside of the range specified in section 5, the 
display should display the characters ‘ERR’ in place of the demanded speed 
for 2 seconds +/- 0.5 second. The display should then revert to the value 
displayed before the out-of-range speed was entered.
2 Keypad:
2.1 All keyboard switching should be fully debounced.
2.2 A key is accepted as ‘pressed’ at the point when the debounce is complete if 
it is determined that the transitions on the keyboard matrix are consistent 
with an actual keypress.
2.3 If more than one key is pressed at the same time, the first key to be detected 
as pressed is the one to be accepted.
2.4 Numerical values should be set by the operator via the keypad.
2.5 The ‘#’ key is an ‘enter’ key – the value entered by the operator will be accepted by the firmware once this key is pressed.
2.6 The ‘*’ key is the backspace key – this will delete the previous digit entered. If 
no digits are entered, this key will do nothing.
3 7-segment LED display
3.1 The 7-segment LED display should display the current key being pressed, for 
the duration for which it is pressed. ‘#’ should be displayed as ‘E’, while ‘*’ 
should be displayed as ‘b’.
3.2 When no key is pressed, the decimal point should be illuminated.
3.3 The decimal point must be extinguished for the duration for which a key is 
pressed.
4 Analog pot
4.1 This is not used in this specification.
5 Push switches S1 and S2
5.1 These are not used in this specification but may be used if you wish for any 
additional purpose as desired by the designer (e.g. debugging).
6 Individual LEDs
6.1 Individual LEDs are not required for this specification, and thus may be used 
for any purpose as desired by the designer (e.g. debugging).
7 Control
7.1 On power up/reset:
7.1.1 The message “Starting…” should be displayed on the screen until the 
firmware is fully initialized. This period should not exceed 2 seconds, 
but may be less than 2 seconds.
7.1.2 The date/time may be set by hardcoding it during initialization. It does 
not need to be the actual date and time - it may be hardcoded to any 
valid date and time to allow testing of the datalogger
7.1.3 Once initialized for the first time, the display should behave as 
required by section 1
7.1.4 A valid date and time should be programmed into the real-time clock 
and the display should read in accordance with section 2.
7.1.5 the motor should be stationary (not rotating).
7.1.6 the 2x16 display should read zero for both demanded and actual 
speed.
7.1.7 the 7-segment display should be blank, with the decimal point 
illuminated.
7.2 Motor speed control:
7.2.1 Motor speed control should be achieved using open-loop control.
7.2.2 Maximum displayed speed error (displayed demanded value to actual 
value) is 10%. Some slack will be allowed in this number to account 
for the different motors in different development boards having 
substantially different characteristics.
7.2.3 Maximum actual speed error (as measured by an external 
tachometer) is 10%
7.2.4 Minimum motor speed is 50 revolutions per second (rps).
7.2.5 Maximum motor speed is given by the maximum speed possible when 
the motor is driven from a PWM source with a duty cycle of 1. This will 
have to be determined experimentally by the designer.
7.2.6 Operators shall not be able to enter a speed out of this range without 
the ‘ERR’ message being displayed (section 6) and the value being 
rejected.
8 Data Logging
8.1 Data is to be logged once per second when the open-loop speed controller is 
active, i.e. for demanded speeds greater than zero. The logging can be 
suspended for the duration where the demanded speed is zero.
8.2 A log entry will be made every 2 seconds, and additionally once at the point 
when a new speed value is entered by the keypad.
8.3 Each logged entry will record the day, month, year, hour, minute, second, , 
demanded speed, actual speed.
8.4 Each logged entry will be written to the external I2C EEPROM.
8.5 Once the demanded speed is set to 0 RPS, the data logging will stop, and 
the logged data will be sent to the serial port. The date should be in a humanreadable comma separated variable (CSV) format, while the demanded 
speed and actual speed may form part of the textual log message The 
contents of which should be able to be cut and pasted from the serial 
terminal and saved as a text file with .CSV extension. Such a file should be 
able to be imported into spreadsheet packages such as Excel.
8.6 The external EEPROM contains a finite storage space. Once the storage is 
filled, the datalogger should act like a flight recorder; i.e. ‘wrap around’ and 
overwrite from the beginning, but do so without causing corruption of the 
store.
9 Firmware restrictions
9.1 The firmware shall be constructed in such a manner as to facilitate ease of 
expansion with additional I/O.
9.2 The provided kernel must be used.
9.3 Languages:
9.3.1 C or C++ should be used. Some functions may be written in machine 
code using AVR assembler language where appropriate.
>>>>>>> 30188d82974071a20ae904f5d3c60ac1cef15886
