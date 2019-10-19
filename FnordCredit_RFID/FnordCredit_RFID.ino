#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Keyboard.h>

Adafruit_PN532 nfc(10);

#define SERIAL_BAUDRATE 115200

void setup() {

    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FnordCredit RFID Reader by DevCoffee");

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.print("Didn't find PN53x board");
        while (1); // halt
    }

    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    nfc.setPassiveActivationRetries(0xFF);

    // configure board to read RFID tags
    nfc.SAMConfig();

    Serial.println("PN532 Ready");

    Keyboard.begin();

}

void loop() {


    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength)) {
        Serial.println("Found a card!");
        Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("UID Value: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(" 0x"); Serial.print(uid[i], HEX);
        }
        Serial.println("");

        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('t');
        Keyboard.releaseAll();

        //Keyboard print RFID UID
        for (uint8_t i = 0; i < uidLength; i++) {
            String out = String(uid[i],HEX);
            Keyboard.print(out);
        }
        Keyboard.write(KEY_RETURN);
        // Wait 1 second before continuing
        delay(1000);
    } else {
        // PN532 probably timed out waiting for a card
        Serial.println("Timed out waiting for a card");
    }
}
