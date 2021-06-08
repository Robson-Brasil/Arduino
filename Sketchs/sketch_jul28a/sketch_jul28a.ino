#include <WiFi.h>
 
const char* ssid = "SSID";
const char* password =  "PASSWORD";
 
void setup() {
 
 int timeout = 0;
  
 Serial.begin(115200);
 WiFi.begin(ssid);
  
 delay(2000);
  
 Serial.print("Connecting ESP32 to ");
 Serial.print(ssid);
 Serial.println(' '); 
  
 while (WiFi.status() != WL_CONNECTED) {
     
    delay(500);
    timeout++;
  EasyIot
    Serial.print('.');
    if(timeout == 60){
        timeout = 0;
        Serial.print('\n');
        Serial.println("Connection failed!");
        Serial.println("Trying again");
        Serial.print('\n');
        Serial.print("Connecting ESP32 to EasyIot");
        Serial.print(ssid);
        Serial.print(' '); 
    }
   
 }
  Serial.print('\n');
  Serial.println("Connected!");
  Serial.print("My IP:\t");
  Serial.println(WiFi.localIP());
}
  
void loop() {}
