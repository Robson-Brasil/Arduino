<<<<<<< HEAD

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX
#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleSerialBLE.h>

#include <dht.h> // Inclui a biblioteca no seu código
int  DHTPIN  =  A1;

dht DHT; // Cria um objeto da classe dht
uint32_t timer = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PXcLbQrnASpT-556y0ganpQXOuhjNYoA";

void setup()
{
  // Debug console
  DebugSerial.begin(38400);

  DebugSerial.println("Waiting for connections...");

  pinMode(DHTPIN, OUTPUT);

  // Blynk will work through Serial
  // 9600 is for HC-06. For HC-05 default speed is 38400
  // Do not read or write this serial manually in your sketch
  Serial.begin(38400);
  Blynk.begin(Serial, auth);
}

void loop()
{
  Blynk.run();
   
}
=======

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX
#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleSerialBLE.h>

#include <dht.h> // Inclui a biblioteca no seu código
int  DHTPIN  =  A1;

dht DHT; // Cria um objeto da classe dht
uint32_t timer = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PXcLbQrnASpT-556y0ganpQXOuhjNYoA";

void setup()
{
  // Debug console
  DebugSerial.begin(38400);

  DebugSerial.println("Waiting for connections...");

  pinMode(DHTPIN, OUTPUT);

  // Blynk will work through Serial
  // 9600 is for HC-06. For HC-05 default speed is 38400
  // Do not read or write this serial manually in your sketch
  Serial.begin(38400);
  Blynk.begin(Serial, auth);
}

void loop()
{
  Blynk.run();
   
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
