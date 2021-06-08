<<<<<<< HEAD
//Bibliotecas utilizadas
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED_conexao       2
#define Rele1            32
#define Rele2             5
#define Rele3            18
#define Rele4            19
#define LDR              34
#define buzzer           23

#define WLAN_SSID       "MOTOROLA-A7CE9"      // Nome da Rede Wifi
#define WLAN_PASS       "3104e43ece65b0885fa9"     // Senha da Rede Wifi
 
/************************* Adafruit.io Configuração *********************************/
 
#define AIO_SERVER      "io.adafruit.com" //Adafruit Servidor
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "RobsonBrasil"      //Insira o usuario criado na adafruit io
#define AIO_KEY         "aio_wMWB961cAgaZvcqOHyE71aIkT4E1" //Insira a chave de comunicação obtida na adafruit io
 
//Váriáveis utilizadas
 
int conectado = 1;
int conexao = 0;
int alarme = 0;
int trava = 0;
 
unsigned long anterior = 0;
unsigned long intervalo = 15000;
unsigned long atual;
 
//Definição do Wifi Client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
//Váriáveis criadas para comunicação com o MQTT
Adafruit_MQTT_Publish   sensor = Adafruit_MQTT_Publish  (&mqtt, AIO_USERNAME "/feeds/Status Alarme");   // Crie aqui sua variavel
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 1");        // a palavra feeds deve estar em todos
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 2");    
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 4");
Adafruit_MQTT_Subscribe Light5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 5");
 
//Funções
void MQTT_connect(); //Determina a conexão MQTT
void Conectar();     //Verifica a conexão o Wifi
 
void setup() {
 
  Serial.begin(115200);
 
  //Definição de saida e entrada de cada pino
  pinMode(LED_conexao,OUTPUT);
  pinMode(Rele1,OUTPUT);
  pinMode(Rele2,OUTPUT);
  pinMode(Rele3,OUTPUT);
  pinMode(Rele4, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(LDR,INPUT);
 
  //Desliga o LED de status de conexão do Wifi
  digitalWrite(LED_conexao,LOW);
   
  //Declaração de Tópicos de Leitura
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
  mqtt.subscribe(&Light5);
 
}
 
void loop() {
 
  Conectar();
  MQTT_connect();
 
  //Leitura do valor recebido através da Adafruit IO
  Adafruit_MQTT_Subscribe *subscription;
  if((subscription = mqtt.readSubscription(100))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Rele1, Light1_State);
    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Rele2, Light2_State);
    }
    if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(Rele3, Light3_State);
    }
    if (subscription == &Light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      digitalWrite(Rele4, Light4_State);
    }
    if (subscription == &Light5) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light5.lastread);
      int Light5_State = atoi((char *)Light5.lastread);
      alarme = Light5_State;
    }
  }
  if(alarme){
    //Leitura do LDR
    int leitura = analogRead(LDR);
    Serial.println(leitura);
    if(leitura < 2900){
      trava = 1;
      //Buzzer tocando intermitente
      digitalWrite(buzzer,HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
      delay(500);
    }
    atual = millis();
    if (atual - anterior > intervalo)
    {    
        if(trava == 1){
          //Tópico de publicação de acionamento do sensor
          sensor.publish(1);
          trava = 0;
        }
        else{
          //Tópico de publicação de desacionamento do sensor
          sensor.publish(0);
        }
        anterior = atual;
     }
  }
  else{
     //Desliga Buzzer
     digitalWrite(buzzer,LOW);
     trava = 0;
     atual = millis();
     if (atual - anterior > intervalo)
     {   
          sensor.publish(0);
          anterior = atual;
     }
  }
}
 
void MQTT_connect() {
  int8_t ret;
  //Conectando MQTT
  if (mqtt.connected()) {
    if(conectado){
      conectado = 0;
      Serial.println("MQTT Conectado!");
    }
    return;
  }
  Serial.print("Conectando MQTT...");
  uint8_t retries = 3;
  if((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Reconexao ao MQTT em 5 segundos...");
    mqtt.disconnect();
    delay(5000);
    conectado = 1;
    //retries--;
    if (retries == 0) {
      //retries = 3;
    }
  }
}
 
void Conectar(){
  //Verifica a conexão wifi
  if(WiFi.status() != WL_CONNECTED){
      conexao = 0; 
      //Conectando ao Wifi
      Serial.println();
      Serial.println();
      Serial.print("Conectando a rede: ");
      Serial.println(WLAN_SSID);
      //Inicializa Conexão Wifi
      WiFi.begin(WLAN_SSID, WLAN_PASS);
      while(WiFi.status() != WL_CONNECTED) {
            digitalWrite(LED_conexao,LOW);
            delay(100);
            digitalWrite(LED_conexao,HIGH);
            delay(100);
            Serial.print(".");
            conexao++;
            if(conexao == 30){
              ESP.restart();
            }
      }
      Serial.println("WiFi conectado!");
      Serial.println("Endereco de IP: ");
      Serial.println(WiFi.localIP());
      digitalWrite(LED_conexao,HIGH);
      conectado = 1;
      Serial.println();
  }
}
=======
//Bibliotecas utilizadas
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED_conexao       2
#define Rele1            32
#define Rele2             5
#define Rele3            18
#define Rele4            19
#define LDR              34
#define buzzer           23

#define WLAN_SSID       "MOTOROLA-A7CE9"      // Nome da Rede Wifi
#define WLAN_PASS       "3104e43ece65b0885fa9"     // Senha da Rede Wifi
 
/************************* Adafruit.io Configuração *********************************/
 
#define AIO_SERVER      "io.adafruit.com" //Adafruit Servidor
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "RobsonBrasil"      //Insira o usuario criado na adafruit io
#define AIO_KEY         "aio_wMWB961cAgaZvcqOHyE71aIkT4E1" //Insira a chave de comunicação obtida na adafruit io
 
//Váriáveis utilizadas
 
int conectado = 1;
int conexao = 0;
int alarme = 0;
int trava = 0;
 
unsigned long anterior = 0;
unsigned long intervalo = 15000;
unsigned long atual;
 
//Definição do Wifi Client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
//Váriáveis criadas para comunicação com o MQTT
Adafruit_MQTT_Publish   sensor = Adafruit_MQTT_Publish  (&mqtt, AIO_USERNAME "/feeds/Status Alarme");   // Crie aqui sua variavel
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 1");        // a palavra feeds deve estar em todos
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 2");    
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 4");
Adafruit_MQTT_Subscribe Light5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Rele 5");
 
//Funções
void MQTT_connect(); //Determina a conexão MQTT
void Conectar();     //Verifica a conexão o Wifi
 
void setup() {
 
  Serial.begin(115200);
 
  //Definição de saida e entrada de cada pino
  pinMode(LED_conexao,OUTPUT);
  pinMode(Rele1,OUTPUT);
  pinMode(Rele2,OUTPUT);
  pinMode(Rele3,OUTPUT);
  pinMode(Rele4, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(LDR,INPUT);
 
  //Desliga o LED de status de conexão do Wifi
  digitalWrite(LED_conexao,LOW);
   
  //Declaração de Tópicos de Leitura
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
  mqtt.subscribe(&Light5);
 
}
 
void loop() {
 
  Conectar();
  MQTT_connect();
 
  //Leitura do valor recebido através da Adafruit IO
  Adafruit_MQTT_Subscribe *subscription;
  if((subscription = mqtt.readSubscription(100))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Rele1, Light1_State);
    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Rele2, Light2_State);
    }
    if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(Rele3, Light3_State);
    }
    if (subscription == &Light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      digitalWrite(Rele4, Light4_State);
    }
    if (subscription == &Light5) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light5.lastread);
      int Light5_State = atoi((char *)Light5.lastread);
      alarme = Light5_State;
    }
  }
  if(alarme){
    //Leitura do LDR
    int leitura = analogRead(LDR);
    Serial.println(leitura);
    if(leitura < 2900){
      trava = 1;
      //Buzzer tocando intermitente
      digitalWrite(buzzer,HIGH);
      delay(500);
      digitalWrite(buzzer,LOW);
      delay(500);
    }
    atual = millis();
    if (atual - anterior > intervalo)
    {    
        if(trava == 1){
          //Tópico de publicação de acionamento do sensor
          sensor.publish(1);
          trava = 0;
        }
        else{
          //Tópico de publicação de desacionamento do sensor
          sensor.publish(0);
        }
        anterior = atual;
     }
  }
  else{
     //Desliga Buzzer
     digitalWrite(buzzer,LOW);
     trava = 0;
     atual = millis();
     if (atual - anterior > intervalo)
     {   
          sensor.publish(0);
          anterior = atual;
     }
  }
}
 
void MQTT_connect() {
  int8_t ret;
  //Conectando MQTT
  if (mqtt.connected()) {
    if(conectado){
      conectado = 0;
      Serial.println("MQTT Conectado!");
    }
    return;
  }
  Serial.print("Conectando MQTT...");
  uint8_t retries = 3;
  if((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Reconexao ao MQTT em 5 segundos...");
    mqtt.disconnect();
    delay(5000);
    conectado = 1;
    //retries--;
    if (retries == 0) {
      //retries = 3;
    }
  }
}
 
void Conectar(){
  //Verifica a conexão wifi
  if(WiFi.status() != WL_CONNECTED){
      conexao = 0; 
      //Conectando ao Wifi
      Serial.println();
      Serial.println();
      Serial.print("Conectando a rede: ");
      Serial.println(WLAN_SSID);
      //Inicializa Conexão Wifi
      WiFi.begin(WLAN_SSID, WLAN_PASS);
      while(WiFi.status() != WL_CONNECTED) {
            digitalWrite(LED_conexao,LOW);
            delay(100);
            digitalWrite(LED_conexao,HIGH);
            delay(100);
            Serial.print(".");
            conexao++;
            if(conexao == 30){
              ESP.restart();
            }
      }
      Serial.println("WiFi conectado!");
      Serial.println("Endereco de IP: ");
      Serial.println(WiFi.localIP());
      digitalWrite(LED_conexao,HIGH);
      conectado = 1;
      Serial.println();
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
