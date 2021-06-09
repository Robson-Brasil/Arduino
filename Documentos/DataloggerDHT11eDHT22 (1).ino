//Incluindo Bibliotecas
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "RTClib.h"
#include <SD.h>

//define objetos RTC e File
RTC_DS1307 rtc;
File file;


//Pino CS do cartao SD
int Pino_CS = 10;
//Define Pinos dos sensores DHT
#define DHTPIN A0      
#define DHTPIN A1      

// Define tipos de sensor utilizados:
#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

//Cria os sensores com os seus tipos
DHT_Unified dht(A0, DHT11);
DHT_Unified dht1(A1, DHT22);

//Define tempo de delay
uint32_t delayMS;

//Declarando pinos do lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  
  Serial.begin(9600);
  // inicializa sensores.
  dht.begin();
    dht1.begin();
    
//Iniciando o LCD e declarando numero de linhas e colunas do lcd
  lcd.begin(16, 2);
  
  // Definir o tempo do delay.
  delayMS =  2000;
//imprime no monitor serioal o nome do projeto
  Serial.println("Data logger com sensor de temperatura DHT11 e DHT22");
  Serial.println();
  
//Inicia o cartao SD
  Serial.println("Iniciando cartao SD...");
  if (!SD.begin(Pino_CS))
  {
    Serial.println("Falha na inicializacao do SD!");
    return;
  }
  Serial.println("Cartao SD iniciado. OK");
  Serial.println();

  //Verifica as condicoes do RTC
  if (! rtc.begin())
  {
    Serial.println("RTC nao encontrado!");
    while (1);
  }
  if (! rtc.isrunning())
  {
    Serial.println("RTC nao operante!");
    //A linha abaixo ajusta o RTC com a data e hora do momento da compilacao
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //A linha abaixo permite ajustar manualmente a data e hora do RTC
    //Formato: DateTime(ano, mes, dia, hora, minuto, segundo)
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}
void loop(){
{
  // Delay para iniciar a leitura.
  delay(delayMS);
  //Le as informacoes do RTC
  DateTime now = rtc.now();
  //Serial monitor informacoes de hora
  Serial.print(now.hour() < 10 ? "0" : "");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute() < 10 ? "0" : "");
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second() < 10 ? "0" : "");
  Serial.print(now.second(), DEC);
  Serial.println();
  
  // Delay antes da leituraDHT0.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
    Serial.println(F("Error reading temperature!"));
  
  else 
    Serial.print(F("Temp0: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
    Serial.println(F("Error reading humidity!"));
  
  else 
    Serial.print(F("Hum0: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    Serial.println();


//Telling our lcd to refresh itself every 0.75 seconds
  lcd.clear();
 
  //Choosing the first line and row
  lcd.setCursor(0,0);
  //Typing Temp: to the first line starting from the first row
  lcd.print(F("Temp0: "));
  //Typing the temperature readings after "Temp: " 
  lcd.print(F("°C"));
  //Choosing the second line and first row
  lcd.setCursor(0,1);
  //Typing Humidity(%): to the second line starting from the first row
  lcd.print("Hum0: ");
  //Typing the humidity readings after "Humidity(%): "
  lcd.print("Hum0: ");



}

{
    // Delay antes de iniciar a leitura do DHT1.
  delay(delayMS);
   //Le as informacoes do RTC
  DateTime now = rtc.now();
  //Serial monitor informacoes de hora
  Serial.print(now.hour() < 10 ? "0" : "");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute() < 10 ? "0" : "");
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second() < 10 ? "0" : "");
  Serial.print(now.second(), DEC);
  Serial.println();
  // Get temperature event and print its value.
  sensors_event_t event;
  dht1.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
    Serial.println(F("Error reading temperature!"));
  
  else 
    Serial.print(F("Temp1: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  
  // Get humidity event and print its value.
  dht1.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
    Serial.println(F("Error reading humidity!"));
  
  else 
    Serial.print(F("Hum1: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    Serial.println();




//Telling our lcd to refresh itself every 0.75 seconds
  lcd.clear();
 
  //Choosing the first line and row
  lcd.setCursor(0,0);
  //Typing Temp: to the first line starting from the first row
  lcd.print("Temp1: ");
  //Typing the temperature readings after "Temp: " 
  lcd.print(event.temperature);
  //Choosing the second line and first row
  lcd.setCursor(0,1);
  //Typing Humidity(%): to the second line starting from the first row
  lcd.print("Hum1: ");
  //Typing the humidity readings after "Humidity(%): "
  lcd.print(event.relative_humidity);



    
}
  /*

//Gravacao do cartao
  //Abre arquivo no SD para gravacao
  file = SD.open("dados.csv", FILE_WRITE);
  //Grava os dados no cartao SD
  file.print(now.hour() < 10 ? "0" : "");
  file.print(now.hour(), DEC);
  file.print(':');
  file.print(now.minute() < 10 ? "0" : "");
  file.print(now.minute(), DEC);
  file.print(':');
  file.print(now.second() < 10 ? "0" : "");
  file.print(now.second(), DEC);
  file.print(",");
  file.print(t, 0);
  file.println();
  //Fecha arquivo
  file.close();
  //Delay ate a proxima leitura
  delay(10000);*/

  } 
  
  
