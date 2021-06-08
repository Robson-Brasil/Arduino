<<<<<<< HEAD
#define BLYNK_PRINT Serial // Blynk Serial Print
#include <WiFi.h> // Wi-Fi
#include <WiFiClient.h> // Wi-Fi client 
#include <BlynkSimpleEsp32.h> // Blynk-ESP32
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define RELAY_1 22 //pino controla o relé


//-------- Token de Autenticação -----------
char auth[] = "Zac4szr95VgI1wr82uigeKXUavwRnaAy";
//-------- Configurações de Wi-Fi -----------
char ssid[] = "MOTOROLA-A7CE9";
char pass[] = "3104e43ece65b0885fa9";
//-------- Pino Virtual -----------
BLYNK_CONNECTED()
{         // Se conectar com Blynk
  Blynk.syncVirtual(V1);    // Sincroniza com o pino virtual V1 
  Blynk.syncVirtual(V2);    // Sincroniza com o pino V2
  Blynk.syncVirtual(V3);    // Sincroniza com o pino V3
}
void setup()
{
  Serial.begin(115200);     // Taxa de transmissão 115200
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, LOW);
  pinMode(23, OUTPUT);           // LED Azul
  pinMode(4, OUTPUT);           // LED Verde
  pinMode(2, OUTPUT);           // LED Vermelho
  Blynk.begin(auth, ssid, pass); // TOKEN+REDE+SENHA
}
void loop() 
{
  
  Blynk.run();
  
}
=======
#define BLYNK_PRINT Serial // Blynk Serial Print
#include <WiFi.h> // Wi-Fi
#include <WiFiClient.h> // Wi-Fi client 
#include <BlynkSimpleEsp32.h> // Blynk-ESP32
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define RELAY_1 22 //pino controla o relé


//-------- Token de Autenticação -----------
char auth[] = "Zac4szr95VgI1wr82uigeKXUavwRnaAy";
//-------- Configurações de Wi-Fi -----------
char ssid[] = "MOTOROLA-A7CE9";
char pass[] = "3104e43ece65b0885fa9";
//-------- Pino Virtual -----------
BLYNK_CONNECTED()
{         // Se conectar com Blynk
  Blynk.syncVirtual(V1);    // Sincroniza com o pino virtual V1 
  Blynk.syncVirtual(V2);    // Sincroniza com o pino V2
  Blynk.syncVirtual(V3);    // Sincroniza com o pino V3
}
void setup()
{
  Serial.begin(115200);     // Taxa de transmissão 115200
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, LOW);
  pinMode(23, OUTPUT);           // LED Azul
  pinMode(4, OUTPUT);           // LED Verde
  pinMode(2, OUTPUT);           // LED Vermelho
  Blynk.begin(auth, ssid, pass); // TOKEN+REDE+SENHA
}
void loop() 
{
  
  Blynk.run();
  
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
