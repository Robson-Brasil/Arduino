//IoT - Automação Residencial
//Temperatura, Umidade e Luminosidade
//Autor : Robson Brasil
//Versão : 13
//Última Modificação : 27/08/2020

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

char auth[] = "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "RVR 2,4GHz"; // Enter Your WiFi Name
char pass[] = "RodrigoValRobson2021"; // Enter Your Passwword

// GPIO where the DS18B20 is connected to
const int oneWireBus = 0;   

#define DHTTYPE DHT22
#define DHTPIN 22 // Output Pin Of DHT
int SensorLDR =  23;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;


void sendSensorLDR()
  {
  int LDR = analogRead(SensorLDR);
  if (LDR <2000)
  {
  Serial.begin(115200);
//  digitalWrite(Led, LOW);
  Blynk.notify("Light ON");
}
  }
void sendSensor()
{
  sensors.requestTemperatures(); 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
} 
   
  // or dht.readTemperature(true) for Fahrenheit
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t); // Virtual Pin V5 for Temprature
  Blynk.virtualWrite(V6, h); // Virtual Pin V6 for Humidity
  Blynk.virtualWrite(V8, LDR);//BOTÃO VIRTUAL V8

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  dht.begin();
  sensors.begin();
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendSensorLDR);
  Blynk.begin(auth, ssid, pass);
 
}

void loop(){ 
      {
    Blynk.run();
      timer.run(); // Initiates SimpleTimer
  }
}
