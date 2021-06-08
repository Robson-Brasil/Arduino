<<<<<<< HEAD
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ 6
#define PN532_RESET 3

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {

  Serial.begin(115200); Serial.println("Hello!");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX); 
    }
    Serial.println("");
    
    Serial.print("Value: ");
    unsigned int hex_num;
    for (uint8_t i=0; i < uidLength; i++) 
    {
      if (i == 0) {
        hex_num =  uid[i] << 24; 
      } else if (i == 1) {
        hex_num +=  uid[i] << 16;
      } else if (i == 2) {
        hex_num += uid[i] << 8;
      } else {
        hex_num += uid[i];
      }
    }
    
    Serial.println(hex_num);    
    Serial.println("");
  delay(1000);
  }
  else
  {
    Serial.println("Timed out waiting for a card");
  }
}
=======
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ 6
#define PN532_RESET 3

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {

  Serial.begin(115200); Serial.println("Hello!");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX); 
    }
    Serial.println("");
    
    Serial.print("Value: ");
    unsigned int hex_num;
    for (uint8_t i=0; i < uidLength; i++) 
    {
      if (i == 0) {
        hex_num =  uid[i] << 24; 
      } else if (i == 1) {
        hex_num +=  uid[i] << 16;
      } else if (i == 2) {
        hex_num += uid[i] << 8;
      } else {
        hex_num += uid[i];
      }
    }
    
    Serial.println(hex_num);    
    Serial.println("");
  delay(1000);
  }
  else
  {
    Serial.println("Timed out waiting for a card");
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
