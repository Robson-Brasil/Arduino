<<<<<<< HEAD
/*******************************************************************************
*
*    Projeto - Utilizando um sensor de temperatura e umidade DHT22 com LCD 
*    Autor : Robson Brasil 
*    Versão 1
*    Data a última modificação : 01/05/2021
*
*******************************************************************************/

#include <Adafruit_Sensor.h>                       // Biblioteca DHT Sensor Adafruit 
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>//Carrega a biblioteca lcd com I2C

#define DHTTYPE  DHT11;                           // Sensor DHT11

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 

uint32_t timer = 0;
 
void setup()
{
  Serial.begin(9600); // Inicializa serial com taxa de transmissão de 9600 bauds
  lcd.begin(20, 4); // Define o display com 16 colunas e 2 linhas
  lcd.clear(); // limpa a tela do display
}
 
void loop() {
  // Executa 1 vez a cada 2 segundos
  if(millis() - timer>= 2000)  {

    // chama método de leitura da classe dht conectado no pino A1      
    //DHT.read11(A1); // uso do sensor DHT11
    DHT.read22(A1);   // uso do sensor DHT22            
     
    // Exibe na serial o valor da temperatura
    Serial.print("Temperatura = ");
    Serial.print(DHT.temperature,1); 
    Serial.print(" Celsius  ");
    
    // Exibe na serial o valor de umidade
    Serial.print("Umidade     = ");
    Serial.print(DHT.humidity,1);
    Serial.println(" %  ");

    // Exibe no display LCD o valor da temperatura
    lcd.setCursor(0,0); // Define o cursor na posição de início
    lcd.print("Temperatura = ");
    lcd.print(DHT.temperature,1);
    lcd.write(B11011111); // Imprime o símbolo de grau
    lcd.print("C");

    // Exibe no display LCD o valor da humidade
    lcd.setCursor(0,1); // Define o cursor na posição de início
    lcd.print("Umidade     = ");
    lcd.print(DHT.humidity,1);
    lcd.print(" %");

    // Localiade
    lcd.setCursor(0,2); // Define o cursor na posição de início
    lcd.print("Manaus, AM, Brasil");

    // Localiade
    lcd.setCursor(0,3); // Define o cursor na posição de início
    lcd.print("-3.061266 -60.060650");

    timer = millis(); // Atualiza a referência
  }
} 
=======
/*******************************************************************************
*
*    Projeto - Utilizando um sensor de temperatura e umidade DHT22 com LCD 
*    Autor : Robson Brasil 
*    Versão 1
*    Data a última modificação : 01/05/2021
*
*******************************************************************************/

#include <Adafruit_Sensor.h>                       // Biblioteca DHT Sensor Adafruit 
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>//Carrega a biblioteca lcd com I2C

#define DHTTYPE  DHT11;                           // Sensor DHT11

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 

uint32_t timer = 0;
 
void setup()
{
  Serial.begin(9600); // Inicializa serial com taxa de transmissão de 9600 bauds
  lcd.begin(20, 4); // Define o display com 16 colunas e 2 linhas
  lcd.clear(); // limpa a tela do display
}
 
void loop() {
  // Executa 1 vez a cada 2 segundos
  if(millis() - timer>= 2000)  {

    // chama método de leitura da classe dht conectado no pino A1      
    //DHT.read11(A1); // uso do sensor DHT11
    DHT.read22(A1);   // uso do sensor DHT22            
     
    // Exibe na serial o valor da temperatura
    Serial.print("Temperatura = ");
    Serial.print(DHT.temperature,1); 
    Serial.print(" Celsius  ");
    
    // Exibe na serial o valor de umidade
    Serial.print("Umidade     = ");
    Serial.print(DHT.humidity,1);
    Serial.println(" %  ");

    // Exibe no display LCD o valor da temperatura
    lcd.setCursor(0,0); // Define o cursor na posição de início
    lcd.print("Temperatura = ");
    lcd.print(DHT.temperature,1);
    lcd.write(B11011111); // Imprime o símbolo de grau
    lcd.print("C");

    // Exibe no display LCD o valor da humidade
    lcd.setCursor(0,1); // Define o cursor na posição de início
    lcd.print("Umidade     = ");
    lcd.print(DHT.humidity,1);
    lcd.print(" %");

    // Localiade
    lcd.setCursor(0,2); // Define o cursor na posição de início
    lcd.print("Manaus, AM, Brasil");

    // Localiade
    lcd.setCursor(0,3); // Define o cursor na posição de início
    lcd.print("-3.061266 -60.060650");

    timer = millis(); // Atualiza a referência
  }
} 
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
