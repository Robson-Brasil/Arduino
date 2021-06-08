<<<<<<< HEAD
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

// define the GPIO connected with Relays and switches
#define RelayPin1 34  //D23
#define RelayPin2 35  //D22
#define RelayPin3 32  //D21
#define RelayPin4 33  //D19

#define SwitchPin1 21  //D13
#define SwitchPin2 13  //D12
#define SwitchPin3 12  //D14
#define SwitchPin4 22  //D27

#define wifiLed    25   //D2

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
              }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);   // Update Button Widget
    }
}
void without_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
    }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
  }
}
void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
=======
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

// define the GPIO connected with Relays and switches
#define RelayPin1 34  //D23
#define RelayPin2 35  //D22
#define RelayPin3 32  //D21
#define RelayPin4 33  //D19

#define SwitchPin1 21  //D13
#define SwitchPin2 13  //D12
#define SwitchPin3 12  //D14
#define SwitchPin4 22  //D27

#define wifiLed    25   //D2

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

#define AUTH "WW7eZUxja-yaJ04fANp6KF-z0N1K63Jg"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "RVR 2,4GHz"             //Enter Wifi Name
#define WIFI_PASS "RodrigoValRobson2021"         //Enter wifi Password

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
              }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);   // Update Button Widget
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);   // Update Button Widget
    }
}
void without_internet(){
    //Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW){
      delay(200);
      relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW){
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW){
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW){
      delay(200);
      relayOnOff(4);
    }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
  }
}
void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
