# FnordCredit RFID-Reader
Adds support for user authentication using RFID Tag UIDs to [FnordCredit](https://github.com/fnordcredit/fnordcredit)

## Hardware
This Project uses an __Arduino Pro Micro (ATmega32u4)__ as its microcontroller due to it being cheap and having hardware USB support.
As the RFID reader the more expensive __PN532__ is used due to better software support and broader tag compatibility.

In the future a seperate build with support for the MFRC522 is planned. 

### Wiring
The PN532 is connected with the Pro Micro using its native SPI peripheral pins (14 [MISO], 16[MOSI], 15[SCK], 10[CS]).

![alt text](https://github.com/TimTower01/RFID-Reader/blob/master/Images/FnordCredit%20RFID%20Reader%20Wiring%20Diagram.png "PN532 Wiring Diagram")


## Software
This Project was written in C/C++ using the Arduino IDE.

It uses the [Adafruit PN532 library](https://github.com/adafruit/Adafruit-PN532) for accessing the RFID reader.
For parsing the results to FnordCredit it uses the integrated [Arduino Keyboard library](https://github.com/arduino-libraries/Keyboard).

The Programm reads the 4 or 7 bit long UID of the RFID tag and parses it to FnordCredit by first sending __[LEFT_ALT + T]__ on the keyboard then printing out the __UID in HEX__ followed by a __[RETURN]__.

### Flashing
The programm can be flashed to the microcontroller using [avrdude](https://www.nongnu.org/avrdude/) with the provided precompiled binary.
The following terminal command can be used:
`avrdude -p m32U4 -P /dev/ttyACM0 -c avr109 -U flash:w:FnordCredit_RFID.hex`
You may have to replace `/dev/ttyACM0` with your correspondig serial port.

If you want to compile it yourself, just clone this repository and open the `.ino`using the Arduino IDE.
In the Boardmanager select the `Arduino Leonardo`and your serial port.
