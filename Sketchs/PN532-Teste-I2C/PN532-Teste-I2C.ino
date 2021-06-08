<<<<<<< HEAD
#include <Wire.h> //
#include <PN532_I2C.h> //Set PN532 i2C Library can be downloaded below.
#include <PN532.h>   //Library can be dowloaded below
#include <NfcAdapter.h> //Library can be dowloaded below
 
PN532_I2C pn532(Wire); // Set PN532 runs on i2C Procotol
 
NfcAdapter nfc = NfcAdapter(pn532);  
 void setup(void) {
  Serial.begin(115200);
  Serial.println("14CORE | NFC - i2C Test Code TAG Reader");
  Serial.println("Initializing please wait.......");
  delay(1000);
  nfc.begin();
}
 
void loop(void) {
 
  Serial.println("Please wait while scanning any NFC device");  
  delay(2000);
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString()); 
 
    if (tag.hasNdefMessage()) //set if there is message in the tags
    {
 
      NdefMessage message = tag.getNdefMessage();
      Serial.print("NFC TAG MSG: ");
      Serial.print(message.getRecordCount());
      Serial.print("NFC Tag Data");
 
      if (message.getRecordCount() != 1) { 
        Serial.print(".");
      }
 
      Serial.println("Failed No NFC Device Found");
      delay(1000);
 
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
     
      {
        Serial.print("NDEFRecord ");
        Serial.println(i+1);
        NdefRecord record = message.getRecord(i);
 
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
 
        String valString = "";
        for (int c = 0; c < payloadLength; c++) {
          valString += (char)payload[c];
        }
 
        Serial.print("INFO (as String): ");
        Serial.println(valString);
 
 
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");
          Serial.println(uid); // print UID of the NFC Tag
        }
      }
    }
  }
  delay(2000);
}
=======
#include <Wire.h> //
#include <PN532_I2C.h> //Set PN532 i2C Library can be downloaded below.
#include <PN532.h>   //Library can be dowloaded below
#include <NfcAdapter.h> //Library can be dowloaded below
 
PN532_I2C pn532(Wire); // Set PN532 runs on i2C Procotol
 
NfcAdapter nfc = NfcAdapter(pn532);  
 void setup(void) {
  Serial.begin(115200);
  Serial.println("14CORE | NFC - i2C Test Code TAG Reader");
  Serial.println("Initializing please wait.......");
  delay(1000);
  nfc.begin();
}
 
void loop(void) {
 
  Serial.println("Please wait while scanning any NFC device");  
  delay(2000);
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString()); 
 
    if (tag.hasNdefMessage()) //set if there is message in the tags
    {
 
      NdefMessage message = tag.getNdefMessage();
      Serial.print("NFC TAG MSG: ");
      Serial.print(message.getRecordCount());
      Serial.print("NFC Tag Data");
 
      if (message.getRecordCount() != 1) { 
        Serial.print(".");
      }
 
      Serial.println("Failed No NFC Device Found");
      delay(1000);
 
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
     
      {
        Serial.print("NDEFRecord ");
        Serial.println(i+1);
        NdefRecord record = message.getRecord(i);
 
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
 
        String valString = "";
        for (int c = 0; c < payloadLength; c++) {
          valString += (char)payload[c];
        }
 
        Serial.print("INFO (as String): ");
        Serial.println(valString);
 
 
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");
          Serial.println(uid); // print UID of the NFC Tag
        }
      }
    }
  }
  delay(2000);
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
