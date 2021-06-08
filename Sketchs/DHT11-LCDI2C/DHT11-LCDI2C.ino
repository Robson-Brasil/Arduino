<<<<<<< HEAD
// Arduino - Sensor DHT - Umidade e Temperatura
// https://blog.eletrogate.com/
// Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_Sensor.h>                       // Biblioteca DHT Sensor Adafruit 
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>//Carrega a biblioteca lcd com I2C

// selecione um sensor, retirando o comentário - duas barras
#define DHTTYPE    DHT11                           // Sensor DHT11
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

//#define DHTTYPE      DHT22                       // Sensor DHT22 ou AM2302
#define DHTPIN 2                                   // Pino do Arduino conectado no Sensor(Data) 
DHT_Unified dht(DHTPIN, DHTTYPE);                  // configurando o Sensor DHT - pino e tipo
uint32_t delayMS;                                  // variável para atraso no tempo
void setup()
{
  Serial.begin(9600);                             // monitor serial 9600 bps
  dht.begin();                                    // inicializa a função
  Serial.println("Usando o Sensor DHT");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);           // imprime os detalhes do Sensor de Temperatura
  Serial.println("------------------------------------");
  Serial.println("Temperatura");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");

  // Exibe no display LCD o valor da temperatura
  lcd.setCursor(0,0); // Define o cursor na posição de início
  lcd.print("Temperatura = ");
  lcd.print(DHT.temperature,1);
  lcd.write(B11011111); // Imprime o símbolo de grau
  lcd.print("C");
  
  dht.humidity().getSensor(&sensor);            // imprime os detalhes do Sensor de Umidade
  Serial.println("------------------------------------");
  Serial.println("Umidade");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;            // define o atraso entre as leituras

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

  Serial.begin(9600); // Inicializa serial com taxa de transmissão de 9600 bauds
  lcd.begin(20, 4); // Define o display com 16 colunas e 2 linhas
  lcd.clear(); // limpa a tela do display
}
void loop()
{
  delay(delayMS);                               // atraso entre as medições
  sensors_event_t event;                        // inicializa o evento da Temperatura
  dht.temperature().getEvent(&event);           // faz a leitura da Temperatura
  if (isnan(event.temperature))                 // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Temperatura!");
  }
  else                                          // senão
  {
    Serial.print("Temperatura: ");              // imprime a Temperatura
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);              // faz a leitura de umidade
  if (isnan(event.relative_humidity))           // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Umidade!");
  }
  else                                          // senão
  {
    Serial.print("Umidade: ");                  // imprime a Umidade
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
=======
// Arduino - Sensor DHT - Umidade e Temperatura
// https://blog.eletrogate.com/
// Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_Sensor.h>                       // Biblioteca DHT Sensor Adafruit 
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>//Carrega a biblioteca lcd com I2C

// selecione um sensor, retirando o comentário - duas barras
#define DHTTYPE    DHT11                           // Sensor DHT11
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

//#define DHTTYPE      DHT22                       // Sensor DHT22 ou AM2302
#define DHTPIN 2                                   // Pino do Arduino conectado no Sensor(Data) 
DHT_Unified dht(DHTPIN, DHTTYPE);                  // configurando o Sensor DHT - pino e tipo
uint32_t delayMS;                                  // variável para atraso no tempo
void setup()
{
  Serial.begin(9600);                             // monitor serial 9600 bps
  dht.begin();                                    // inicializa a função
  Serial.println("Usando o Sensor DHT");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);           // imprime os detalhes do Sensor de Temperatura
  Serial.println("------------------------------------");
  Serial.println("Temperatura");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");

  // Exibe no display LCD o valor da temperatura
  lcd.setCursor(0,0); // Define o cursor na posição de início
  lcd.print("Temperatura = ");
  lcd.print(DHT.temperature,1);
  lcd.write(B11011111); // Imprime o símbolo de grau
  lcd.print("C");
  
  dht.humidity().getSensor(&sensor);            // imprime os detalhes do Sensor de Umidade
  Serial.println("------------------------------------");
  Serial.println("Umidade");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Valor max:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Valor min:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolucao:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;            // define o atraso entre as leituras

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

  Serial.begin(9600); // Inicializa serial com taxa de transmissão de 9600 bauds
  lcd.begin(20, 4); // Define o display com 16 colunas e 2 linhas
  lcd.clear(); // limpa a tela do display
}
void loop()
{
  delay(delayMS);                               // atraso entre as medições
  sensors_event_t event;                        // inicializa o evento da Temperatura
  dht.temperature().getEvent(&event);           // faz a leitura da Temperatura
  if (isnan(event.temperature))                 // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Temperatura!");
  }
  else                                          // senão
  {
    Serial.print("Temperatura: ");              // imprime a Temperatura
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);              // faz a leitura de umidade
  if (isnan(event.relative_humidity))           // se algum erro na leitura
  {
    Serial.println("Erro na leitura da Umidade!");
  }
  else                                          // senão
  {
    Serial.print("Umidade: ");                  // imprime a Umidade
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
