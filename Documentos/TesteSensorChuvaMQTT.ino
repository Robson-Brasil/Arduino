// Teste sensor de chuva no Node-Red 

#include <WiFi.h> 
#include <PubSubClient.h> 
#include "EspMQTTClient.h" 

#define LED 12
#define pino_a 14    // Pino A0 do módulo conectado ao GPIO 14 do esp32 
//#define pino_d 27 

int val_a = 0; 
int val_d = 0; 
int valorLimiteChuva = 2000; 
bool chuva; 

const char* ssid = "ssid"; 
const char* password = "senha"; 
const char* mqtt_server = "192.168.x.x";  

static const char* connectionString = ""; 
long lastTemp = 0; 

static bool hasIoTHub = false; 

WiFiClient espClient; 
PubSubClient client(espClient); 

void setup() 
{
  Serial.begin(115200); 
  setup_wifi(); 
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); 
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Mensagem recebida ["); 
  Serial.print(topic); 
  Serial.print("]"); 
  for(int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]); 
  } 
  Serial.println(); 
  if((char)payload[0] == '0') 
  {
    Serial.println("LOW"); 
    digitalWrite(LED, LOW); 
  } 
  if((char)payload[0] == '1') 
  {
    Serial.println("HIGH"); 
    digitalWrite(LED, HIGH); 
  } 
} 

void setup_wifi() 
{
  delay(10); 
  Serial.println(); 
  Serial.print("Conectado a rede: "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, password); 
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.println("WiFi Conectado"); 
  Serial.println(WiFi.localIP()); 
} 

void reconnect() 
{
  while(!client.connected()) 
  {
    Serial.print("Conectando ao MQTT..."); 
    // Conectando 
    if(client.connect("ESP32Client")) 
    {
      Serial.println("Conectado"); // Conectado 
      client.publish("event", "Teste Sensor de Chuva Conectado"); 
      // Envia mensagem ao servidor 
      client.subscribe("event"); 
    } 
    else 
    {
      Serial.print("Erro:"); 
      Serial.print(client.state()); 
      Serial.println("Reconectando em 5 segundos"); 
      // Espera 5 segundos e reconecta 
      delay(5000); 
    }
  }
}

int sensorDeChuva() 
{
  int valorSensorChuva = analogRead(pino_a); 
  Serial.print("Sensor de chuva = "); 
  Serial.print(valorSensorChuva); 
  if(valorSensorChuva < valorLimiteChuva) 
  {
    Serial.println(" => Esta chovendo"); 
    chuva = 1; 
  } 
  else 
  {
    Serial.println(" => O tempo esta seco"); 
    chuva = 0; 
  } 
  return valorSensorChuva; 
}

void loop() 
{
  if(!client.connected()) 
  {
    reconnect(); 
  }
  client.loop(); 
  long now = millis(); 
  if(now - lastTemp > 2000) 
  {
    lastTemp = now; 
    sensorDeChuva(); 
    int res = sensorDeChuva(); 
    //val_d = digitalRead(pino_d); 
    //val_a = analogRead(pino_a); 
    //Serial.println(val_a); 
    //Serial.println(val_d); 
    char ChuvaString[8]; 
    dtostrf(res, 1, 2, ChuvaString); 
    Serial.print("Sensor de chuva: "); 
    Serial.println(ChuvaString); 
    client.publish("event", ChuvaString); 
  }
}
