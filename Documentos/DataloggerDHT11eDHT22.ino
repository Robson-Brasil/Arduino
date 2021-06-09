// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "RTClib.h"
#include <SD.h>
//Pino CS do cartao SD
int Pino_CS = 10;

RTC_DS1307 rtc;
File file;


#define DHTPIN A0     // Digital pin connected to the DHT sensor 
#define DHTPIN A1     // Digital pin connected to the DHT sensor 

// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(A0, DHT11);
DHT_Unified dht1(A1, DHT22);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
    dht1.begin();

  // Set delay between sensor readings based on sensor details.
  delayMS =  2000;

  Serial.println("Data logger com sensor de temperatura DHT11");
  Serial.println();
  
//Inicia o cartao SD
  Serial.println("Iniciando cartao SD...");
  if (!SD.begin(Pino_CS))
  {
    Serial.println("Falha na inicializacao do SD!");
    return;
  }
  Serial.println("Cartao SD iniciado. OK");
  Serial.println();

  //Verifica as condicoes do RTC
  if (! rtc.begin())
  {
    Serial.println("RTC nao encontrado!");
    while (1);
  }
  if (! rtc.isrunning())
  {
    Serial.println("RTC nao operante!");
    //A linha abaixo ajusta o RTC com a data e hora do momento da compilacao
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //A linha abaixo permite ajustar manualmente a data e hora do RTC
    //Formato: DateTime(ano, mes, dia, hora, minuto, segundo)
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}
void loop(){
{
  // Delay between measurements.
  delay(delayMS);
  //Le as informacoes do RTC
  DateTime now = rtc.now();
  //Serial monitor informacoes de hora
  Serial.print(now.hour() < 10 ? "0" : "");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute() < 10 ? "0" : "");
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second() < 10 ? "0" : "");
  Serial.print(now.second(), DEC);
  }
  
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temp0 - c: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Hum0 - %: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));



    // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht1.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temp1- c: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht1.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Hum1 - %: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  
  

//Gravacao do cartao
  //Abre arquivo no SD para gravacao
  file = SD.open("dados.csv", FILE_WRITE);
  //Grava os dados no cartao SD
  file.print(now.hour() < 10 ? "0" : "");
  file.print(now.hour(), DEC);
  file.print(':');
  file.print(now.minute() < 10 ? "0" : "");
  file.print(now.minute(), DEC);
  file.print(':');
  file.print(now.second() < 10 ? "0" : "");
  file.print(now.second(), DEC);
  file.print(",");
  file.print(t, 0);
  file.println();
  //Fecha arquivo
  file.close();
  //Delay ate a proxima leitura
  delay(10000);
}
  } 
  
  }
