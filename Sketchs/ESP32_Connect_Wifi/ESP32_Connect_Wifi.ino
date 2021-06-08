<<<<<<< HEAD
#include <WiFi.h>;

const char* ssid = "RVR 2,4GHz";
const char* password =  "RodrigoValRobson2021";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
  {
    Serial.println("You can try to ping me");
    delay(5000);
  }
  else
  {
    Serial.println("Connection lost");
  }
}
=======
#include <WiFi.h>;

const char* ssid = "RVR 2,4GHz";
const char* password =  "RodrigoValRobson2021";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
  {
    Serial.println("You can try to ping me");
    delay(5000);
  }
  else
  {
    Serial.println("Connection lost");
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
