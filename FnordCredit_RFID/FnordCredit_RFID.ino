#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Keyboard.h>

//Create a PN532 instance with pin 10 as chip-select
Adafruit_PN532 nfc(10);

//#define DEBUG

#ifdef DEBUG
   #define SERIAL_BAUDRATE 115200
#endif

void setup() {

    #ifdef DEBUG
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FnordCredit RFID Reader by DevCoffee");
    #endif

    //Initialize the PN532 chip
    nfc.begin();

    //Check for presence of a PN532 on the SPI bus
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        #ifdef DEBUG
        Serial.print("Didn't find PN53x board");
        #endif
        while (1); // halt
    }

    //Print out the chip type and firmware version
    #ifdef DEBUG
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
    #endif

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    nfc.setPassiveActivationRetries(0xFF);

    // configure board to read RFID tags
    nfc.SAMConfig();

    #ifdef DEBUG
    Serial.println("PN532 Ready");
    #endif

    //Initialize the Keyboard to be able to send the UID to FnordCredit
    Keyboard.begin();

}

void loop() {

    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength)) {
        #ifdef DEBUG
        Serial.println("Found a card!");
        Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("UID Value: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(" 0x"); Serial.print(uid[i], HEX);
        }
        Serial.println("");
        #endif

        //Start the communication sequence by sending [LEFT_ALT + T] on the virtual keyboard
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('t');
        Keyboard.releaseAll();

        //Send the UID of the RFID card byte by byte in hexadecimal over the virtual keyboard.
        for (uint8_t i = 0; i < uidLength; i++) {
            String out = String(uid[i], HEX);
            Keyboard.print(out);
        }

        //End the sequence by sending a [RETURN]
        Keyboard.write(KEY_RETURN);

        //Add a delay befor continuing
        delay(1000);
    } else {
        //PN532 probably timed out waiting for a card
        #ifdef DEBUG
        Serial.println("Timed out waiting for a card");
        #endif
    }
}
