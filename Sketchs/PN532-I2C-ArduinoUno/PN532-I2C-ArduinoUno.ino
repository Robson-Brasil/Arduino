<<<<<<< HEAD
*/

#include <Wire.h>

#include <PN532_I2C.h>

#include <PN532.h>

#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);

NfcAdapter nfc = NfcAdapter(pn532_i2c);

/* Uno's A4 to SDA & A5 to SCL */

void setup(void) {

    Serial.begin(115200);

    Serial.println("NDEF Reader");

    nfc.begin();

}

void loop(void) {

    Serial.println("\nScan a NFC tag\n");

    if (nfc.tagPresent())

    {

        NfcTag tag = nfc.read();

        tag.print();

    }

    delay(5000);

}
=======
*/

#include <Wire.h>

#include <PN532_I2C.h>

#include <PN532.h>

#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);

NfcAdapter nfc = NfcAdapter(pn532_i2c);

/* Uno's A4 to SDA & A5 to SCL */

void setup(void) {

    Serial.begin(115200);

    Serial.println("NDEF Reader");

    nfc.begin();

}

void loop(void) {

    Serial.println("\nScan a NFC tag\n");

    if (nfc.tagPresent())

    {

        NfcTag tag = nfc.read();

        tag.print();

    }

    delay(5000);

}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
