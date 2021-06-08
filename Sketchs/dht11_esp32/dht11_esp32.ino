<<<<<<< HEAD
#include <Easyiot-Esp32.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

const char*  ssid = "MEU_SSID"; // <-- Digite aqui o nome da sua rede WiFi
const char* password = "MINHA_SENHA"; // <-- Digite aqui a senha da sua rede WiFi
String EasyToken = "MEU_TOKEN"; // <-- Cole aqui seu Easy Token, obtido na plataforma https://app.easyiot.com.br
int intervalo_de_transmissao = 10000; // <-- Intervalo em milissegundos(10.000 ms == 10s)
  
#define DHTPIN  4 // Pino em que foi conectado a saida dos dados do sensor ao Esp8266
#define DHTTYPE DHT11 // Tipo do sensor DHT -> 11
  
DHT dht(DHTPIN, DHTTYPE); //Declara o objeto dht
  
void setup() {
  int timeout = 0;
  
  Serial.begin(9600);
  delay(5000);
  Serial.print("Conectando a ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) || (timeout < 10)) {
    Serial.print(".");
    timeout++;
    delay(1000);
    if(timeout == 30){
        timeout = 0;
        Serial.println();
        Serial.println("Falha na conexão, verifique os dados da sua WiFi!");
        Serial.println("Tentando conectar novamente");
        Serial.println();
        Serial.print("Conectando a ");
        Serial.print(ssid); 
    }
  }
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  dht.begin(); //Inicializa o sensor
}
  
void loop()  {
  String Dados, resposta;
  
  float h = dht.readHumidity(); //Realiza a leitura da umidade e armazena na variavel h
  float t = dht.readTemperature(); //Realiza a leitura da temperatura e armazena na variavel t
  // testa se retorno é valido
  if (isnan(t) || isnan(h))  {
    Serial.println("Erro ao ler dados do sensor :("); // Retorno invalido, logo ocorreu erro
  } 
  else {
  
    Dados = "Umidade: ["; // A mensagem que sera enviada para a plataforma contendo a temperatura e umidade medidas
    Dados = Dados + h; 
    Dados = Dados + "]%    Temperatura: [";
    Dados = Dados + t;
    Dados = Dados + "] *C";
  


    resposta = send(Dados, EasyToken); //A resposta do servidor estará no variavel resposta
  }
  
  delay(intervalo_de_transmissao);
}
=======
#include <Easyiot-Esp32.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

const char*  ssid = "MEU_SSID"; // <-- Digite aqui o nome da sua rede WiFi
const char* password = "MINHA_SENHA"; // <-- Digite aqui a senha da sua rede WiFi
String EasyToken = "MEU_TOKEN"; // <-- Cole aqui seu Easy Token, obtido na plataforma https://app.easyiot.com.br
int intervalo_de_transmissao = 10000; // <-- Intervalo em milissegundos(10.000 ms == 10s)
  
#define DHTPIN  4 // Pino em que foi conectado a saida dos dados do sensor ao Esp8266
#define DHTTYPE DHT11 // Tipo do sensor DHT -> 11
  
DHT dht(DHTPIN, DHTTYPE); //Declara o objeto dht
  
void setup() {
  int timeout = 0;
  
  Serial.begin(9600);
  delay(5000);
  Serial.print("Conectando a ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) || (timeout < 10)) {
    Serial.print(".");
    timeout++;
    delay(1000);
    if(timeout == 30){
        timeout = 0;
        Serial.println();
        Serial.println("Falha na conexão, verifique os dados da sua WiFi!");
        Serial.println("Tentando conectar novamente");
        Serial.println();
        Serial.print("Conectando a ");
        Serial.print(ssid); 
    }
  }
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  dht.begin(); //Inicializa o sensor
}
  
void loop()  {
  String Dados, resposta;
  
  float h = dht.readHumidity(); //Realiza a leitura da umidade e armazena na variavel h
  float t = dht.readTemperature(); //Realiza a leitura da temperatura e armazena na variavel t
  // testa se retorno é valido
  if (isnan(t) || isnan(h))  {
    Serial.println("Erro ao ler dados do sensor :("); // Retorno invalido, logo ocorreu erro
  } 
  else {
  
    Dados = "Umidade: ["; // A mensagem que sera enviada para a plataforma contendo a temperatura e umidade medidas
    Dados = Dados + h; 
    Dados = Dados + "]%    Temperatura: [";
    Dados = Dados + t;
    Dados = Dados + "] *C";
  


    resposta = send(Dados, EasyToken); //A resposta do servidor estará no variavel resposta
  }
  
  delay(intervalo_de_transmissao);
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
