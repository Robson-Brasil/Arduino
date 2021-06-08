<<<<<<< HEAD
//IoT - Automação Residencial
//Temperatura, Umidade, Luminosidade e Módulo Relé
//Autor : Robson Brasil
//Versão : 13
//Última Modificação : 27/08/2020

// --- Bibliotecas Auxiliares ---
#include <WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// --- Mapeamento de Hardware ---
#define         BLYNK_PRINT Serial
int DHTTYPE  =  DHT22;
int DHTPIN   =     23;
int SensorLDR =    34;
//int Led      =     23;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int Status1   =      1;
int pushButton1State = HIGH;
int Status2   =      1;
int pushButton2State = HIGH;
int Status3   =      1;
int pushButton3State = HIGH;
int Status4   =      1;
int pushButton4State = HIGH;
int Status5   =      1;
int pushButton5State = HIGH;

int wifiFlag = 0;

#define auth "H7GMtj6DSNGqwNMq_FUGhq0cn-DDQC_j"//Seu Token de Autenticação do Aplicativo Blynk.
#define ssid "MOTOROLA-A7CE9"//Nome da Rede Wi-fi
#define pass "3104e43ece65b0885fa9"//Senha da Rede Wi-Fi

int Rele1     =     15;//D13-GPIO 15
int Rele2     =      4;//D12-GPIO 4
int Rele3     =     14;//D14-GPIO 14
int Rele4     =     27;//D27-GPIO 27
int Rele5     =     18;//D27-GPIO 18
int Rele6     =      5;//D21-gpio 21

int WIFI_LED  =      2;//D2-LED INTERNO

int Botao1    =     26;//D26-GPIO 26
int Botao2    =     25;//D25-GPIO 25
int Botao3    =     33;//D33-GPIO 33
int Botao4    =     32;//D32-GPIO 32
int Botao5    =     19;//D32-GPIO 19

#define Virtual1    V1//BOTÃO VIRTUAL V1
#define Virtual2    V2//BOTÃO VIRUTAL V2
#define Virtual3    V3//BOTÃO VIRUTAL V3
#define Virtual4    V4//BOTÃO VIRTUAL V4
#define Virtual5    V5//BOTÃO VIRTUAL V5
#define Virtual6    V6//BOTÃO VIRTUAL V6
#define Virtual7    V7//BOTÃO VIRTUAL V7
#define Virtual8    V8//BOTÃO VIRTUAL V8

// --- Protótipo das Funções Auxiliares ---
void relayOnOff(int relay){
  
    switch(relay){
      case 1: 
             if(Status1 == 0){
              digitalWrite(Rele1, HIGH);//Ligar Relé 1
              Status1 = 1;
              }
             else{
              digitalWrite(Rele1, LOW);//Desligar Relé 1
              Status1 = 0;
              }
             delay(250);
      break;
      case 2: 
             if(Status2 == 0){
              digitalWrite(Rele2, HIGH);//Ligar Relé 2
              Status2 = 1;
              }
             else{
              digitalWrite(Rele2, LOW);//Desligar Relé 2
              Status2 = 0;
              }
             delay(250);
      break;
      case 3: 
             if(Status3 == 0){
              digitalWrite(Rele3, HIGH);//Ligar Relé 3
              Status3 = 1;
              }
             else{
              digitalWrite(Rele3, LOW);//Desligar Relé 3
              Status3 = 0;
              }
             delay(250);
      break;
      case 4: 
             if(Status4 == 0){
              digitalWrite(Rele4, HIGH);//Ligar Relé 4
              Status4 = 1;
              }
             else{
              digitalWrite(Rele4, LOW);//Desligar Relé 4
              Status4 = 0;
              }
             delay(250);
      break;
      case 5: 
             if(Status5 == 0){
              digitalWrite(Rele5, HIGH);//Ligar Relé 5
              Status5 = 1;
              }
             else{
              digitalWrite(Rele5, LOW);//Desligar Relé 5
              Status5 = 0;
              }
             delay(250);
      break;
      default : break;      
      } 
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(Virtual1);//Relé 1
  Blynk.syncVirtual(Virtual2);//Relé 2
  Blynk.syncVirtual(Virtual3);//Relé 3
  Blynk.syncVirtual(Virtual4);//Relé 4
  Blynk.syncVirtual(Virtual5);//Relé 5
  Blynk.syncVirtual(Virtual6);//DHT22 - Temperatura
  Blynk.syncVirtual(Virtual7);//DHT22 - Umidade
  Blynk.syncVirtual(Virtual8);//LDR
}
BLYNK_WRITE(Virtual1){
  Status1 = param.asInt();
  digitalWrite(Rele1, Status1);
}
BLYNK_WRITE(Virtual2){
  Status2 = param.asInt();
  digitalWrite(Rele2, Status2);
}
BLYNK_WRITE(Virtual3){
  Status3 = param.asInt();
  digitalWrite(Rele3, Status3);
}
BLYNK_WRITE(Virtual4){
  Status4 = param.asInt();
  digitalWrite(Rele4, Status4);
}
BLYNK_WRITE(Virtual5){
  Status5 = param.asInt();
  digitalWrite(Rele5, Status5);
}
// --- Protótipo das Funções Auxiliares ---
void with_internet(){
  
  if (digitalRead(Botao1) == LOW) {
      relayOnOff(1);
      Blynk.virtualWrite(Virtual1, Status1);
    }
  if (digitalRead(Botao2) == LOW) {
      relayOnOff(2);
      Blynk.virtualWrite(Virtual2, Status2);
    }
      if (digitalRead(Botao3) == LOW) {
      relayOnOff(3);
      Blynk.virtualWrite(Virtual3, Status3);
    }
  if (digitalRead(Botao4) == LOW) {
      relayOnOff(4);
      Blynk.virtualWrite(Virtual4, Status4);
    }
  if (digitalRead(Botao5) == LOW) {
      relayOnOff(5);
      Blynk.virtualWrite(Virtual5, Status5);
    }
}
// --- Protótipo das Funções Auxiliares ---
void without_internet(){
  
  if (digitalRead(Botao1) == LOW) {
      relayOnOff(1);
    }
  if (digitalRead(Botao2) == LOW) {
      relayOnOff(2);
    }
  if (digitalRead(Botao3) == LOW) {
      relayOnOff(3);
    }
  if (digitalRead(Botao4) == LOW) {
      relayOnOff(4);
    }
  if (digitalRead(Botao5) == LOW) {
      relayOnOff(5);
    }    
}
// --- Protótipo das Funções Auxiliares ---
void checkBlynkStatus() { 
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
      wifiFlag = 1;
      digitalWrite(WIFI_LED, LOW);
   }
  if (isconnected == true) {
      wifiFlag = 0;
      digitalWrite(WIFI_LED, HIGH);
    }
}
// --- Protótipo das Funções Auxiliares ---
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();//Ou dht.readTemperature(true) para temperatura em Fahrenheit
 
  if (isnan(h) || isnan(t) ) {
     Serial.println(F("Failed to read from DHT sensor!"));
     return;
  }
  Blynk.virtualWrite(V6, t);//BOTÃO VIRTUAL V6
  Blynk.virtualWrite(V7, h);//BOTÃO VIRTUAL V7
}
// --- Protótipo das Funções Auxiliares ---
void sendSensorLDR(){
  {
  int LDR = analogRead(SensorLDR);
  if (LDR <2450)
  {
  Serial.begin(115200);
  digitalWrite(Rele6, LOW);
//  digitalWrite(Led, LOW);
  Blynk.notify("Light ON");
}
  else
  {
  digitalWrite(Rele6, HIGH);
//  digitalWrite(Led, HIGH);
}
  Blynk.virtualWrite(V8, LDR);//BOTÃO VIRTUAL V8
  }
}
// --- Configurações Iniciais ---
void setup(){
  
  dht.begin();
  timer.setInterval(1000L, sendSensor);

  timer.setInterval(1000L, sendSensorLDR);

  pinMode(DHTPIN, OUTPUT);
  
//  pinMode(Led, OUTPUT);
  
  pinMode(Rele1, OUTPUT);
  pinMode(Botao1, INPUT_PULLUP);
  digitalWrite(Rele1, Status1);
  
  pinMode(Rele2, OUTPUT);
  pinMode(Botao2, INPUT_PULLUP);
  digitalWrite(Rele2, Status2);
  
  pinMode(Rele3, OUTPUT);
  pinMode(Botao3, INPUT_PULLUP);
  digitalWrite(Rele3, Status3);
  
  pinMode(Rele4, OUTPUT);
  pinMode(Botao4, INPUT_PULLUP);
  digitalWrite(Rele4, Status4);

  pinMode(Rele5, OUTPUT);
  pinMode(Botao5, INPUT_PULLUP);
  digitalWrite(Rele5, Status5);
  
  pinMode(Rele6, OUTPUT);
  
  pinMode(WIFI_LED, OUTPUT);

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  timer.setInterval(3000L, checkBlynkStatus); 
  Blynk.config(auth);
}
// --- Loop Infinito ---
void loop(){  

    {
    Blynk.run();
      timer.run(); // Initiates SimpleTimer
  }
 if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
=======
//IoT - Automação Residencial
//Temperatura, Umidade, Luminosidade e Módulo Relé
//Autor : Robson Brasil
//Versão : 13
//Última Modificação : 27/08/2020

// --- Bibliotecas Auxiliares ---
#include <WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// --- Mapeamento de Hardware ---
#define         BLYNK_PRINT Serial
int DHTTYPE  =  DHT22;
int DHTPIN   =     23;
int SensorLDR =    34;
//int Led      =     23;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int Status1   =      1;
int pushButton1State = HIGH;
int Status2   =      1;
int pushButton2State = HIGH;
int Status3   =      1;
int pushButton3State = HIGH;
int Status4   =      1;
int pushButton4State = HIGH;
int Status5   =      1;
int pushButton5State = HIGH;

int wifiFlag = 0;

#define auth "H7GMtj6DSNGqwNMq_FUGhq0cn-DDQC_j"//Seu Token de Autenticação do Aplicativo Blynk.
#define ssid "MOTOROLA-A7CE9"//Nome da Rede Wi-fi
#define pass "3104e43ece65b0885fa9"//Senha da Rede Wi-Fi

int Rele1     =     15;//D13-GPIO 15
int Rele2     =      4;//D12-GPIO 4
int Rele3     =     14;//D14-GPIO 14
int Rele4     =     27;//D27-GPIO 27
int Rele5     =     18;//D27-GPIO 18
int Rele6     =      5;//D21-gpio 21

int WIFI_LED  =      2;//D2-LED INTERNO

int Botao1    =     26;//D26-GPIO 26
int Botao2    =     25;//D25-GPIO 25
int Botao3    =     33;//D33-GPIO 33
int Botao4    =     32;//D32-GPIO 32
int Botao5    =     19;//D32-GPIO 19

#define Virtual1    V1//BOTÃO VIRTUAL V1
#define Virtual2    V2//BOTÃO VIRUTAL V2
#define Virtual3    V3//BOTÃO VIRUTAL V3
#define Virtual4    V4//BOTÃO VIRTUAL V4
#define Virtual5    V5//BOTÃO VIRTUAL V5
#define Virtual6    V6//BOTÃO VIRTUAL V6
#define Virtual7    V7//BOTÃO VIRTUAL V7
#define Virtual8    V8//BOTÃO VIRTUAL V8

// --- Protótipo das Funções Auxiliares ---
void relayOnOff(int relay){
  
    switch(relay){
      case 1: 
             if(Status1 == 0){
              digitalWrite(Rele1, HIGH);//Ligar Relé 1
              Status1 = 1;
              }
             else{
              digitalWrite(Rele1, LOW);//Desligar Relé 1
              Status1 = 0;
              }
             delay(250);
      break;
      case 2: 
             if(Status2 == 0){
              digitalWrite(Rele2, HIGH);//Ligar Relé 2
              Status2 = 1;
              }
             else{
              digitalWrite(Rele2, LOW);//Desligar Relé 2
              Status2 = 0;
              }
             delay(250);
      break;
      case 3: 
             if(Status3 == 0){
              digitalWrite(Rele3, HIGH);//Ligar Relé 3
              Status3 = 1;
              }
             else{
              digitalWrite(Rele3, LOW);//Desligar Relé 3
              Status3 = 0;
              }
             delay(250);
      break;
      case 4: 
             if(Status4 == 0){
              digitalWrite(Rele4, HIGH);//Ligar Relé 4
              Status4 = 1;
              }
             else{
              digitalWrite(Rele4, LOW);//Desligar Relé 4
              Status4 = 0;
              }
             delay(250);
      break;
      case 5: 
             if(Status5 == 0){
              digitalWrite(Rele5, HIGH);//Ligar Relé 5
              Status5 = 1;
              }
             else{
              digitalWrite(Rele5, LOW);//Desligar Relé 5
              Status5 = 0;
              }
             delay(250);
      break;
      default : break;      
      } 
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(Virtual1);//Relé 1
  Blynk.syncVirtual(Virtual2);//Relé 2
  Blynk.syncVirtual(Virtual3);//Relé 3
  Blynk.syncVirtual(Virtual4);//Relé 4
  Blynk.syncVirtual(Virtual5);//Relé 5
  Blynk.syncVirtual(Virtual6);//DHT22 - Temperatura
  Blynk.syncVirtual(Virtual7);//DHT22 - Umidade
  Blynk.syncVirtual(Virtual8);//LDR
}
BLYNK_WRITE(Virtual1){
  Status1 = param.asInt();
  digitalWrite(Rele1, Status1);
}
BLYNK_WRITE(Virtual2){
  Status2 = param.asInt();
  digitalWrite(Rele2, Status2);
}
BLYNK_WRITE(Virtual3){
  Status3 = param.asInt();
  digitalWrite(Rele3, Status3);
}
BLYNK_WRITE(Virtual4){
  Status4 = param.asInt();
  digitalWrite(Rele4, Status4);
}
BLYNK_WRITE(Virtual5){
  Status5 = param.asInt();
  digitalWrite(Rele5, Status5);
}
// --- Protótipo das Funções Auxiliares ---
void with_internet(){
  
  if (digitalRead(Botao1) == LOW) {
      relayOnOff(1);
      Blynk.virtualWrite(Virtual1, Status1);
    }
  if (digitalRead(Botao2) == LOW) {
      relayOnOff(2);
      Blynk.virtualWrite(Virtual2, Status2);
    }
      if (digitalRead(Botao3) == LOW) {
      relayOnOff(3);
      Blynk.virtualWrite(Virtual3, Status3);
    }
  if (digitalRead(Botao4) == LOW) {
      relayOnOff(4);
      Blynk.virtualWrite(Virtual4, Status4);
    }
  if (digitalRead(Botao5) == LOW) {
      relayOnOff(5);
      Blynk.virtualWrite(Virtual5, Status5);
    }
}
// --- Protótipo das Funções Auxiliares ---
void without_internet(){
  
  if (digitalRead(Botao1) == LOW) {
      relayOnOff(1);
    }
  if (digitalRead(Botao2) == LOW) {
      relayOnOff(2);
    }
  if (digitalRead(Botao3) == LOW) {
      relayOnOff(3);
    }
  if (digitalRead(Botao4) == LOW) {
      relayOnOff(4);
    }
  if (digitalRead(Botao5) == LOW) {
      relayOnOff(5);
    }    
}
// --- Protótipo das Funções Auxiliares ---
void checkBlynkStatus() { 
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
      wifiFlag = 1;
      digitalWrite(WIFI_LED, LOW);
   }
  if (isconnected == true) {
      wifiFlag = 0;
      digitalWrite(WIFI_LED, HIGH);
    }
}
// --- Protótipo das Funções Auxiliares ---
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();//Ou dht.readTemperature(true) para temperatura em Fahrenheit
 
  if (isnan(h) || isnan(t) ) {
     Serial.println(F("Failed to read from DHT sensor!"));
     return;
  }
  Blynk.virtualWrite(V6, t);//BOTÃO VIRTUAL V6
  Blynk.virtualWrite(V7, h);//BOTÃO VIRTUAL V7
}
// --- Protótipo das Funções Auxiliares ---
void sendSensorLDR(){
  {
  int LDR = analogRead(SensorLDR);
  if (LDR <2450)
  {
  Serial.begin(115200);
  digitalWrite(Rele6, LOW);
//  digitalWrite(Led, LOW);
  Blynk.notify("Light ON");
}
  else
  {
  digitalWrite(Rele6, HIGH);
//  digitalWrite(Led, HIGH);
}
  Blynk.virtualWrite(V8, LDR);//BOTÃO VIRTUAL V8
  }
}
// --- Configurações Iniciais ---
void setup(){
  
  dht.begin();
  timer.setInterval(1000L, sendSensor);

  timer.setInterval(1000L, sendSensorLDR);

  pinMode(DHTPIN, OUTPUT);
  
//  pinMode(Led, OUTPUT);
  
  pinMode(Rele1, OUTPUT);
  pinMode(Botao1, INPUT_PULLUP);
  digitalWrite(Rele1, Status1);
  
  pinMode(Rele2, OUTPUT);
  pinMode(Botao2, INPUT_PULLUP);
  digitalWrite(Rele2, Status2);
  
  pinMode(Rele3, OUTPUT);
  pinMode(Botao3, INPUT_PULLUP);
  digitalWrite(Rele3, Status3);
  
  pinMode(Rele4, OUTPUT);
  pinMode(Botao4, INPUT_PULLUP);
  digitalWrite(Rele4, Status4);

  pinMode(Rele5, OUTPUT);
  pinMode(Botao5, INPUT_PULLUP);
  digitalWrite(Rele5, Status5);
  
  pinMode(Rele6, OUTPUT);
  
  pinMode(WIFI_LED, OUTPUT);

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  timer.setInterval(3000L, checkBlynkStatus); 
  Blynk.config(auth);
}
// --- Loop Infinito ---
void loop(){  

    {
    Blynk.run();
      timer.run(); // Initiates SimpleTimer
  }
 if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
