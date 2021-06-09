/*
 * 
Youtube: https://www.instagram.com/voidlooprobotech/
Website: https://www.voidlooprobotech.com 
Twitter: https://twitter.com/SandipRoboTix
Facebook: https://www.facebook.com/groups/289579082069596/
Fcebook: https://www.facebook.com/groups/228864595179720/
Facebook: https://www.facebook.com/groups/2518809408247417/
Facebook: https://www.facebook.com/groups/voidloop/
Facebook: https://www.facebook.com/voidlooprobo/
Instagram: https://www.instagram.com/voidlooprobotech/
Telegram: https://t.me/voidloopnodemcu
Telegram: https://t.me/voidlooprobotech
Whatsapp: https://chat.whatsapp.com/IIyB2IyjyQ71eSfn1jnl3c
 * 
 */


#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define led D1
#define sensor A0
char auth[] = "PF1zb_GCKcm6uL713uhw99U-iG7eVTmU";
char ssid[] = "voidloop";
char pass[] = "shivam1234";

BlynkTimer timer;


void setup()
{
  // Debug console
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, sendSensor);

}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendSensor()
{
  int LDR = analogRead(sensor);
    if(LDR <150)
  {
    digitalWrite(led, HIGH);
    Blynk.notify("Light ON");
  }
  else
  {
    digitalWrite(led, LOW);
   
  }

  Blynk.virtualWrite(V5, LDR);

}
