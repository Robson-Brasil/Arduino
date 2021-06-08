#if defined(ESP8266)
#include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library         
#else
#include <WiFi.h>      //ESP32 Core WiFi Library    
#endif

//needed for library
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal ( https://github.com/zhouhan0126/DNSServer---esp32 )
#if defined(ESP8266)
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#else
#include <WebServer.h> //Local WebServer used to serve the configuration portal ( https://github.com/zhouhan0126/WebServer-esp32 )
#endif
#include <WiFiManager.h>   // WiFi Configuration Magic ( https://github.com/zhouhan0126/WIFIMANAGER-ESP32 ) >> https://github.com/tzapu/WiFiManager (ORIGINAL)

//flag para indicar se foi salva uma nova configuração de rede
bool shouldSaveConfig = false;

//pino do botão
const int PIN_AP = 2;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  //declaração do objeto wifiManager
  WiFiManager wifiManager;

  //utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
//  wifiManager.resetSettings();

//por padrão as mensagens de Debug vão aparecer no monitor serial, caso queira desabilitá-la
//utilize o comando setDebugOutput(false);
//  wifiManager.setDebugOutput(false);

//caso queira iniciar o Portal para se conectar a uma rede toda vez, sem tentar conectar 
//a uma rede salva anteriormente, use o startConfigPortal em vez do autoConnect
//  wifiManager.startConfigPortal(char const *apName, char const *apPassword = NULL);

  //setar IP fixo para o ESP (deve-se setar antes do autoConnect)
//  setAPStaticIPConfig(ip, gateway, subnet);
//  wifiManager.setAPStaticIPConfig(IPAddress(192,168,16,2), IPAddress(192,168,16,1), IPAddress(255,255,255,0)); //modo AP

//  setSTAStaticIPConfig(ip, gateway, subnet);
//  wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); //modo estação

//callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 

  wifiManager.autoConnect("ESP_AP","12345678"); //cria uma rede sem senha
//wifiManager.autoConnect(); //gera automaticamente o SSID com o chip ID do ESP e sem senha

//  wifiManager.setMinimumSignalQuality(10); // % minima para ele mostrar no SCAN

//wifiManager.setRemoveDuplicateAPs(false); //remover redes duplicadas (SSID iguais)

//wifiManager.setConfigPortalTimeout(10); //timeout para o ESP nao ficar esperando para ser configurado para sempre

}

void loop() {
  WiFiManager wifiManager;
  //se o botão foi pressionado
   if ( digitalRead(PIN_AP) == HIGH ) {
      Serial.println("resetar"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("ESP_AP", "12345678") ){
        Serial.println("Falha ao conectar");
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      Serial.println("Conectou ESP_AP!!!");
   }

}

//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede

}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}
