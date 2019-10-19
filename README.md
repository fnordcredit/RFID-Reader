# FnordCredit RFID-Reader
Adds support for user authentication using RFID Tag UIDs to FnordCredit

## Hardware
This Project uses an Arduino Pro Micro (ATmega32u4) as its microcontroller due to it being cheap and having hardware USB support.
As the RFID reader the more expensive PN532 is used due to better software support and broader tag compatibility.

In the future a seperate build with support for the MFRC522 is planned. 

### Wiring
The PN532 is connected with the Pro Micro using its native SPI peripheral pins (14 [MISO], 16[MOSI], 15[SCK]).

TODO: Exact wiring diagramm (Fritzing?)


## Software
This Project was written in C/C++ using the Arduino IDE.

It uses the Adafruit PN532 library for accessing the RFID reader.
For parsing the results to FnordCredit it uses the integrated Arduino Keyboard library.

The Programm reads the 4 or 7 bit long UID of the RFID tag and parses it to FnordCredit by first sending [LEFT_ALT + T] on the keyboard then printing out the UID in HEX followed by a [RETURN].

### Flashing
Flash using avrdude or compile through Arduino IDE.

TODO: Describe avrdude flashing process and prerequisites for compiling and flashing using the Arduini IDE.
