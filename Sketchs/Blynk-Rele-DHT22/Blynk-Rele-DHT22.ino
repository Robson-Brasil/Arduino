
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
// You  get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Coloca o Token do Blynk Aqui";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nome da Tua Rede Aqui";
char pass[] = "Senha da Tua Rede Aqui";

#define DHTTYPE DHT22
#define DHTPIN 22
#define relay1 27
#define relay2 26       Seta os pinos que tu quiser aqui
#define relay3 25
#define relay4 33


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
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}


void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  
  
  // Debug console
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