
#include <WiFi.h>
#include <PubSubClient.h>

#define DEBUG

#define L1 27   //pino de saida para acionamento da Lampada rele1
#define ledint LED_BUILTIN

//informações da rede WIFI
const char* ssid = "TP-Link_DD9A";                 //SSID da rede WIFI
const char* password =  "72359686";    //senha da rede wifi

//informações do broker MQTT - Verifique as informações geradas pelo CloudMQTT
const char* mqttServer = "soldier.cloudmqtt.com";
const int mqttPort = 16488;
const char* mqttUser = "ofcjcngw";
const char* mqttPassword = "g0ksv3PP82A3";
const char* mqttTopicSub ="esp/casa";            //tópico

 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(L1, OUTPUT);
  pinMode(ledint, LED_BUILTIN);
  digitalWrite(L1, HIGH);
  digitalWrite(ledint, LOW);
  
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao WiFi..");
    #endif
  }
  #ifdef DEBUG
  Serial.println("Conectado na rede WiFi");
  #endif
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
 
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
 
    }
  }

  //subscreve no tópico
  client.subscribe(mqttTopicSub);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif

  //aciona saída conforme msg recebida 
  if (strMSG == "rele1_on"){         //se msg "rele1_on"
     digitalWrite(L1, LOW);  //coloca saída em LOW para ligar
  }else if (strMSG == "rele1_off"){   //se msg "rele1_off"
     digitalWrite(L1, HIGH);   //coloca saída em HIGH para desligar
  }
   if (strMSG == "rele2_on"){         //se msg "rele1_on"
     digitalWrite(ledint, HIGH);  //coloca saída em LOW para ligar
  }else if (strMSG == "rele2_off"){   //se msg "rele1_off"
     digitalWrite(ledint, LOW);   //coloca saída em HIGH para desligar
  }
}

//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP32Client", mqttUser, mqttPassword) :
                     client.connect("ESP32Client");

    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado!");
      #endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub, 1); //nivel de qualidade: QoS 1
    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      //Aguarda 10 segundos 
      delay(10000);
    }
  }
}
 
void loop() {
  if (!client.connected()) {
    reconect();
  }
  client.loop();
}
