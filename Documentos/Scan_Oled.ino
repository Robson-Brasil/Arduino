#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// NO NodeMCU D1 =>SCK  D2 =>SDA

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int teste = 0;

void limpar(){
display.display();
display.clearDisplay(); 
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0); //Coluna(max 128) , Linha(max 64)  
}

void setup()   {                
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  delay(2000);
  display.clearDisplay();

WiFi.mode(WIFI_STA);
WiFi.disconnect();
delay(100);
limpar();
}

void loop() {
  display.clearDisplay();
  testdrawline();
  display.display();
  delay(2000);
  display.clearDisplay();
  
display.setTextSize(3);
display.print("SCAN...");
int n = WiFi.scanNetworks();
limpar();
if (n == 0){ display.print("SEM"); display.print("Redes");}
else { display.setTextSize(3); display.println(n); display.println(" Redes");}
delay(3000);
limpar();

for (int i = 0; i <= n; ++i){
display.print("    Rede Wifi "); display.println(i);
display.setCursor(0,14); //Coluna(max 128) , Linha(max 64)  
display.println("SSID: " + WiFi.SSID(i));
display.setCursor(0,28); //Coluna(max 128) , Linha(max 64) 
display.println("Sinal: " + String(WiFi.RSSI(i)));
display.setCursor(0,42); //Coluna(max 128) , Linha(max 64) 
display.println("Canal: " + String(WiFi.channel(i)));
display.setCursor(0,56); //Coluna(max 128) , Linha(max 64) 
display.println("MAC:" + WiFi.BSSIDstr(i));
delay(4000);
limpar();
}
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
    delay(1);
  }
  delay(250);
}  
