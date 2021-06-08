<<<<<<< HEAD
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

int toggleState_1 = 1;
int pushButton1State = HIGH;

int toggleState_2 = 1;
int pushButton2State = HIGH;

int wifiFlag = 0;

// --- Mapeamento de Hardware ---
int  BLYNK_PRINT = Serial;
int  DHTTYPE = DHT22;
int  DHTPIN  =  38;

DHT dht(DHTPIN, DHTTYPE);

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

#define RELAY_PIN_1      36   //D12 Pinos funcionais
#define RELAY_PIN_2      37   //D23 Pinos funcionais
#define WIFI_LED         25   //D25

#define PUSH_BUTTON_1    22   //D13 Pinos funcionais
#define PUSH_BUTTON_2    23   //D21 Pinos funcionais

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 0){
              digitalWrite(RELAY_PIN_1, HIGH); // turn on relay 1
              toggleState_1 = 1;
              }
             else{
              digitalWrite(RELAY_PIN_1, LOW); // turn off relay 1
              toggleState_1 = 0;
              }
             delay(200);
      break;
      case 2: 
             if(toggleState_2 == 0){
              digitalWrite(RELAY_PIN_2, HIGH); // turn on relay 2
              toggleState_2 = 1;
              }
             else{
              digitalWrite(RELAY_PIN_2, LOW); // turn off relay 2
              toggleState_2 = 0;
              }
             delay(200);
      break;
      default : break;      
      } 
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RELAY_PIN_1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RELAY_PIN_2, toggleState_2);
}

void with_internet(){
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
      relayOnOff(1);
      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    }
  if (digitalRead(PUSH_BUTTON_2) == LOW) {
      relayOnOff(2);
      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    }
}
void without_internet(){
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
      relayOnOff(1);
    }
  if (digitalRead(PUSH_BUTTON_2) == LOW) {
      relayOnOff(2);
    }
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

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

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();//Ou dht.readTemperature(true) para temperatura em Fahrenheit
  if (isnan(h) || isnan(t)) {
  Serial.begin(115200);
  Serial.println("Falha na leitura do Sensor DHT22!");
  return;
}
  Blynk.virtualWrite(V5, t);//BOTÃO VIRTUAL V5
  Blynk.virtualWrite(V6, h);//BOTÃO VIRTUAL V6
}

void setup()
{
  Serial.begin(9600);

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, toggleState_1);

  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(PUSH_BUTTON_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, toggleState_2);

  pinMode(WIFI_LED, OUTPUT);

  pinMode(DHTPIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);

}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Não Conectado");
  }
  else
  {
    Serial.println(" WiFi Conectado");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
=======
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

int toggleState_1 = 1;
int pushButton1State = HIGH;

int toggleState_2 = 1;
int pushButton2State = HIGH;

int wifiFlag = 0;

// --- Mapeamento de Hardware ---
int  BLYNK_PRINT = Serial;
int  DHTTYPE = DHT22;
int  DHTPIN  =  38;

DHT dht(DHTPIN, DHTTYPE);

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

#define RELAY_PIN_1      36   //D12 Pinos funcionais
#define RELAY_PIN_2      37   //D23 Pinos funcionais
#define WIFI_LED         25   //D25

#define PUSH_BUTTON_1    22   //D13 Pinos funcionais
#define PUSH_BUTTON_2    23   //D21 Pinos funcionais

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 0){
              digitalWrite(RELAY_PIN_1, HIGH); // turn on relay 1
              toggleState_1 = 1;
              }
             else{
              digitalWrite(RELAY_PIN_1, LOW); // turn off relay 1
              toggleState_1 = 0;
              }
             delay(200);
      break;
      case 2: 
             if(toggleState_2 == 0){
              digitalWrite(RELAY_PIN_2, HIGH); // turn on relay 2
              toggleState_2 = 1;
              }
             else{
              digitalWrite(RELAY_PIN_2, LOW); // turn off relay 2
              toggleState_2 = 0;
              }
             delay(200);
      break;
      default : break;      
      } 
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RELAY_PIN_1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RELAY_PIN_2, toggleState_2);
}

void with_internet(){
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
      relayOnOff(1);
      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    }
  if (digitalRead(PUSH_BUTTON_2) == LOW) {
      relayOnOff(2);
      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    }
}
void without_internet(){
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
      relayOnOff(1);
    }
  if (digitalRead(PUSH_BUTTON_2) == LOW) {
      relayOnOff(2);
    }
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

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

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();//Ou dht.readTemperature(true) para temperatura em Fahrenheit
  if (isnan(h) || isnan(t)) {
  Serial.begin(115200);
  Serial.println("Falha na leitura do Sensor DHT22!");
  return;
}
  Blynk.virtualWrite(V5, t);//BOTÃO VIRTUAL V5
  Blynk.virtualWrite(V6, h);//BOTÃO VIRTUAL V6
}

void setup()
{
  Serial.begin(9600);

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, toggleState_1);

  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(PUSH_BUTTON_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, toggleState_2);

  pinMode(WIFI_LED, OUTPUT);

  pinMode(DHTPIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);

}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Não Conectado");
  }
  else
  {
    Serial.println(" WiFi Conectado");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
