//Biblioteca do LCD
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Declarando pinos do lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Fim do LCD

//Shield datalogger +RCT
#include "RTClib.h"
#include <Wire.h>
#include <SD.h>
//Pino CS do cartao SD
int Pino_CS = 10;
//define objetos RTC e File
RTC_DS1307 rtc;
File file;
File dataFile;
DateTime now;
//Fim da Shielt relogio + data logger

//Bilbioteca do DHT
#include <dht.h>
#define dataPin A0// Define numero dos pinos dos sensores
dht DHT; // cria os objetos DJT
//Fim do DHT

//Define Posição dos Reles
int rele1 = 7 ;
int rele2 = 6 ;

void setup() {

  Serial.begin(9600);
  //Iniciando o LCD e declarando numero de linhas e colunas do lcd
  lcd.begin(16, 2);
  pinMode ( rele1, OUTPUT) ;
  pinMode ( rele2, OUTPUT) ;

  //Inicia parte do SD E RTC
  //imprime no monitor serioal o nome do projeto
  Serial.println("Data logger com sensor de temperatura DHT11 e DHT22");
  Serial.println();
  //Inicia o cartao SD
  Serial.println("Iniciando cartao SD...");
  lcd.print("Iniciando cartao SD...");
    lcd.clear();
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
    
    Serial.println("RTC não operante!");
    lcd.print("RTC não operante!");
    lcd.clear();
    //A linha abaixo ajusta o RTC com a data e hora do momento da compilacao
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //A linha abaixo permite ajustar manualmente a data e hora do RTC
    //Formato: DateTime(ano, mes, dia, hora, minuto, segundo)
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  //Fim parte do SD E RTC

}

void loop() {


  //Le as informacoes do RTC
  DateTime now = rtc.now();
  //Serial monitor data
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.println();
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
//LCD info

  //LCD monitor data
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  //LCD monitor informacoes de hora
  lcd.setCursor(0, 1);
  lcd.print(now.hour() < 10 ? "0" : "");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute() < 10 ? "0" : "");
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second() < 10 ? "0" : "");
  lcd.print(now.second(), DEC);

  // Delay antes da leituraDHT0.
  delay(2000);

  int readData = DHT.read22(A0); // Reads the data from the sensor
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity

  // Printing the results on the serial monitor
  Serial.print("T0 = ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("H0 = ");
  Serial.print(h);
  Serial.println(" % ");

  //Print no LCD
  //Telling our lcd to refresh itself every 0.75 seconds
  lcd.clear();
  //Choosing the first line and row
  lcd.setCursor(0, 0);
  //Typing Temp: to the first line starting from the first row
  lcd.print(F("Temp oc: "));
  //Typing the temperature readings after "Temp: "
  lcd.print(t);
  //Choosing the second line and first row
  lcd.setCursor(0, 1);
  //Typing Humidity(%): to the second line starting from the first row
  lcd.print("Umidade %: ");
  //Typing the humidity readings after "Humidity(%): "
  lcd.print(h);

  //Gravacao do cartao *** GOSTARIA DE GRAVAR A INFORMAÇAO A CADA 10 MIN E NAO C ACADA 2 SEG ***
  //Abre arquivo no SD para gravacao
  file = SD.open("dadosA0.csv", FILE_WRITE);
  //Grava os dados no cartao SD
  //Serial monitor data
  //Serial monitor data
  file.print(now.day(), DEC);
  file.print("/");
  file.print(now.month(), DEC);
  file.print('/');
  file.print(now.year(), DEC);
  file.print(",");
  file.print(now.hour() < 10 ? "0" : "");
  file.print(now.hour(), DEC);
  file.print(':');
  file.print(now.minute() < 10 ? "0" : "");
  file.print(now.minute(), DEC);
  file.print(':');
  file.print(now.second() < 10 ? "0" : "");
  file.print(now.second(), DEC);
  file.print(",");
  file.print(t , 1);
  file.print(",");
  file.print(h , 0);
  file.print(",");
  file.println();
  //Fecha arquivo
  file.close();

  delay(2000); // Delays 2 secods, as the DHT22 sampling rate is 0.5Hz

  {
    if (DHT.temperature > 1 && DHT.temperature < 25 )
    {
      digitalWrite (rele1, HIGH);
      Serial.println(" Rele1 Desligado ");
      digitalWrite (rele2, HIGH);
      Serial.println(" Rele2 Desligado ");

    }
    {
      if (DHT.temperature > 26 && DHT.temperature < 50 )
      {
        digitalWrite (rele1, LOW);
        Serial.println(" Rele1 Ligada ");
        digitalWrite (rele2, LOW);
        Serial.println(" Rele2 Ligado ");

      }
    }
  }
}
