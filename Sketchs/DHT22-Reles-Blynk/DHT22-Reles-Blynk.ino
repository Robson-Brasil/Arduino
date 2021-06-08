<<<<<<< HEAD

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char auth[] = "H7GMtj6DSNGqwNMq_FUGhq0cn-DDQC_j";

char ssid[] = "MOTOROLA-A7CE9";
char pass[] = "3104e43ece65b0885fa9";

#define DHTTYPE DHT22
#define DHTPIN  22
#define Rele1   13
#define Rele2   12
#define Rele3   14
#define Rele4   27

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;

}

  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

void setup()
{
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  digitalWrite(Rele1, HIGH);
  digitalWrite(Rele2, HIGH);
  digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
  
  Serial.begin(115200);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
}
=======

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char auth[] = "H7GMtj6DSNGqwNMq_FUGhq0cn-DDQC_j";

char ssid[] = "MOTOROLA-A7CE9";
char pass[] = "3104e43ece65b0885fa9";

#define DHTTYPE DHT22
#define DHTPIN  22
#define Rele1   13
#define Rele2   12
#define Rele3   14
#define Rele4   27

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;

}

  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

void setup()
{
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  digitalWrite(Rele1, HIGH);
  digitalWrite(Rele2, HIGH);
  digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
  
  Serial.begin(115200);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
