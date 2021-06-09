#include <WiFi.h>
#include "time.h"

String Hora;
const char* ssid       = "xxxxx";
const char* password   = "xxxxx";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;

//nao esqueca de ajustar o fuso
const int   daylightOffset_sec = -3600*3;

void printLocalTime()
{
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter a hora");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S");  
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Conectando em %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" Feito");
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();

}
