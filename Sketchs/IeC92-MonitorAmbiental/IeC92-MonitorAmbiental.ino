<<<<<<< HEAD
/********************************************************
 * CANAL INTERNET E COISAS
 * Projeto de IoT #8
 * Monitor de Condições Ambientais
 * 11/2019 - Andre Michelon
 */

// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <DHT.h>
#include <SPIFFS.h>

// Wi-Fi ------------------------------------------------
const char* ssid        = "MOTOROLA-A7CE9"; // Coloque o nome da sua rede wifi aqui
const char* password    = "3104e43ece65b0885fa9"; // Coloque a sua senha wifi aqui

// Constantes -------------------------------------------
// DHT11
const byte    DHT_PIN                 = 22;
// LDR
const byte    LDR_PIN                 = 33;
// Broker
const char*   MQTT_SERVER             = "internetdascoisas.ddns.net";
// Porta
const int     MQTT_PORT               = 1883;
// Cliente
const char*   MQTT_CLIENT             = "Monitor";
// Topico temperatura
const char*   MQTT_TEMPERATURE_TOPIC  = "Monitor/temperatura";
// Topico umidade
const char*   MQTT_HUMIDITY_TOPIC     = "Monitor/umidade";
// Topico iluminancia
const char*   MQTT_LIGHTING_TOPIC     = "Monitor/iluminacao";
// Topico atualizacao
const char*   MQTT_UPDATE_TOPIC       = "Monitor/atualizacao";
// URL da Data/Hora

// Variaveis globais ------------------------------------

// Temperatura
String        temperature;
// Umidade
String        humidity;
// Iluminação
String        lighting;
// Atualização
String        lastUpdate;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(8221);
DHT dht(DHT_PIN, DHT22);

void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
  //  log("Connectando Broker MQTT...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
  //    log("Conectado");
    } else {
      // Falha na conexao
 //     log("Falha");
      delay(5000);
    }
  }
}

// Processa requisoes HTTP ------------------------------
void handleGetData() {
  // Obtem leitura dos sensores
  String s = temperature + "&" + humidity + "&" + lighting + "&" + lastUpdate;
  server.send(200, "text/plain", s);
 // log("getData " + ipStr(server.client().remoteIP()) + " " + s);
}

void handleDashboard() {
  // Pagina padrao com Dashboard
  File file = SPIFFS.open("/Data/Dashboard.htm", "r");
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
 //   log("Dashboard " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, "text/plain", "ERROR 500 - Dashboard");
 //   log("Dashboard error");
  }
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
 // log("--- Monitor Ambiental ---");

  // SPIFFS
  if (SPIFFS.begin()) {
  //  log("SPIFFS ok");
  } else {
  //  log("SPIFFS falha");
    while(true);
  }

  // Conecta Wi-Fi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
 // log("Conectado IP " + ipStr(WiFi.localIP()));

  // Servidor HTTP
  server.on("/getData", handleGetData);
  server.onNotFound(handleDashboard);
  server.begin();

  // DHT11
  dht.begin();

  // Define Data/Hora
//  setSyncProvider(timeHTTP);

}

// Loop -------------------------------------------------
void loop() {
  // Processa requisicao HTTP
  server.handleClient();

  // Processa conexao ao Broker
  if (client.connected()) {
    client.loop();
  } else {
    reconnect();
  }

  // Atualiza data/hora
 // dateTime = now();

 // if (dateTime > nextDataUpdate) {
    // Atualizar leitura dos sensores
    temperature = String(dht.readTemperature(), 1);
    humidity = String(dht.readHumidity(), 1);
    lighting = String(analogAverage(LDR_PIN) * 100L / 4095);
 //   lastUpdate = dateTimeISO8601(dateTime);

    if (isnan(dht.readTemperature())) 
    
    else {
      // Publica dados no broker
      client.publish(MQTT::Publish(MQTT_TEMPERATURE_TOPIC, temperature)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_HUMIDITY_TOPIC, humidity)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_LIGHTING_TOPIC, lighting)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_UPDATE_TOPIC, lastUpdate)
                      .set_retain(true));
   //   log("Dados atualizados t=" + temperature + "\tu=" + humidity + "\tl=" + lighting);
    }

    // Proxima atualizacao
    nextDataUpdate = dateTime + DATA_UPDATE_INT;
  }
}
=======
/********************************************************
 * CANAL INTERNET E COISAS
 * Projeto de IoT #8
 * Monitor de Condições Ambientais
 * 11/2019 - Andre Michelon
 */

// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <DHT.h>
#include <SPIFFS.h>

// Wi-Fi ------------------------------------------------
const char* ssid        = "MOTOROLA-A7CE9"; // Coloque o nome da sua rede wifi aqui
const char* password    = "3104e43ece65b0885fa9"; // Coloque a sua senha wifi aqui

// Constantes -------------------------------------------
// DHT11
const byte    DHT_PIN                 = 22;
// LDR
const byte    LDR_PIN                 = 33;
// Broker
const char*   MQTT_SERVER             = "internetdascoisas.ddns.net";
// Porta
const int     MQTT_PORT               = 1883;
// Cliente
const char*   MQTT_CLIENT             = "Monitor";
// Topico temperatura
const char*   MQTT_TEMPERATURE_TOPIC  = "Monitor/temperatura";
// Topico umidade
const char*   MQTT_HUMIDITY_TOPIC     = "Monitor/umidade";
// Topico iluminancia
const char*   MQTT_LIGHTING_TOPIC     = "Monitor/iluminacao";
// Topico atualizacao
const char*   MQTT_UPDATE_TOPIC       = "Monitor/atualizacao";
// URL da Data/Hora

// Variaveis globais ------------------------------------

// Temperatura
String        temperature;
// Umidade
String        humidity;
// Iluminação
String        lighting;
// Atualização
String        lastUpdate;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(8221);
DHT dht(DHT_PIN, DHT22);

void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
  //  log("Connectando Broker MQTT...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
  //    log("Conectado");
    } else {
      // Falha na conexao
 //     log("Falha");
      delay(5000);
    }
  }
}

// Processa requisoes HTTP ------------------------------
void handleGetData() {
  // Obtem leitura dos sensores
  String s = temperature + "&" + humidity + "&" + lighting + "&" + lastUpdate;
  server.send(200, "text/plain", s);
 // log("getData " + ipStr(server.client().remoteIP()) + " " + s);
}

void handleDashboard() {
  // Pagina padrao com Dashboard
  File file = SPIFFS.open("/Data/Dashboard.htm", "r");
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
 //   log("Dashboard " + ipStr(server.client().remoteIP()));
  } else {
    server.send(500, "text/plain", "ERROR 500 - Dashboard");
 //   log("Dashboard error");
  }
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
 // log("--- Monitor Ambiental ---");

  // SPIFFS
  if (SPIFFS.begin()) {
  //  log("SPIFFS ok");
  } else {
  //  log("SPIFFS falha");
    while(true);
  }

  // Conecta Wi-Fi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
 // log("Conectado IP " + ipStr(WiFi.localIP()));

  // Servidor HTTP
  server.on("/getData", handleGetData);
  server.onNotFound(handleDashboard);
  server.begin();

  // DHT11
  dht.begin();

  // Define Data/Hora
//  setSyncProvider(timeHTTP);

}

// Loop -------------------------------------------------
void loop() {
  // Processa requisicao HTTP
  server.handleClient();

  // Processa conexao ao Broker
  if (client.connected()) {
    client.loop();
  } else {
    reconnect();
  }

  // Atualiza data/hora
 // dateTime = now();

 // if (dateTime > nextDataUpdate) {
    // Atualizar leitura dos sensores
    temperature = String(dht.readTemperature(), 1);
    humidity = String(dht.readHumidity(), 1);
    lighting = String(analogAverage(LDR_PIN) * 100L / 4095);
 //   lastUpdate = dateTimeISO8601(dateTime);

    if (isnan(dht.readTemperature())) 
    
    else {
      // Publica dados no broker
      client.publish(MQTT::Publish(MQTT_TEMPERATURE_TOPIC, temperature)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_HUMIDITY_TOPIC, humidity)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_LIGHTING_TOPIC, lighting)
                      .set_retain(true));
      client.publish(MQTT::Publish(MQTT_UPDATE_TOPIC, lastUpdate)
                      .set_retain(true));
   //   log("Dados atualizados t=" + temperature + "\tu=" + humidity + "\tl=" + lighting);
    }

    // Proxima atualizacao
    nextDataUpdate = dateTime + DATA_UPDATE_INT;
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
