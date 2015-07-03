PaperTapeReader
---------------

This software is written for Arduino boards and turn it into a
Control unit replacement for FACIT Paper tape reader. In the current
state 4 read modes are available: debug, binary, 7 bit ASCII and
5 bit Baudot.

Connect the digital IO pins 2 to 10 directly to the reader. The pins
2 to 9 are for reading the bits 0 to 7. Pin 10 is for the clock signal
which comes from the feed holes.

To connect to the reader use a serial terminal emulator. With Linux for
example use

screen /dev/ttyUSB0 115200 

If nothing is displayed, press any key. Then select the mode. To
transfer data directly to a file, kill the screen session after
mode selsction. (^a - k) Then start transfer to file with

cat /dev/ttyUSB0 > filename

To finish the transfer press ^C.

In ASCII mode, after reading, the number of read errors will be
displayed. (assuming the 8. bit is used for checksum.)
