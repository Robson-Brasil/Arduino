<<<<<<< HEAD
/********************************************************
 * CANAL INTERNET E COISAS
 * ESP32 - Utilizando HTTP e MQTT
 * 08/2019 - Andre Michelon
 */

// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>

// Wi-Fi ------------------------------------------------
const char* ssid = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";

// Constantes -------------------------------------------
// LED
const byte    LED_PIN             = 2;
// Broker
const char*   MQTT_SERVER         = "internetdascoisas.ddns.net";
// Porta
const int     MQTT_PORT           = 1883;
// Cliente
const char*   MQTT_CLIENT         = "IeCESP32";
// Topico LED Get
const char*   MQTT_LED_GET        = "IeCLED/get";
// Topico LED Set
const char*   MQTT_LED_SET        = "IeCLED/set";
// URL da Data/Hora
const char*   HTTP_TIME_URL       = "https://internetecoisas.com.br/exemplo/iec85.php";
// Fuso horario
const int8_t  TIME_ZONE           = -3;
// Intervalo atualizacao Data/Hora
const int     DATETIME_INT        = 30; // Utilizar 28800 = 8h

// Variaveis globais ------------------------------------
// Data/Hora
time_t        dateTime            = 0;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(8221);

// Funcoes Genericas ------------------------------------
time_t iso8601DateTime(String s, const int8_t tz = 0) {
  // Converte String ISO8601 em time_t
  TimeElements te;
  te.Year   = s.substring( 0,  4).toInt() - 1970;
  te.Month  = s.substring( 5,  7).toInt();
  te.Day    = s.substring( 8, 10).toInt();
  te.Hour   = s.substring(11, 13).toInt();
  te.Minute = s.substring(14, 16).toInt();
  te.Second = s.substring(17, 19).toInt();
  time_t t  = makeTime(te);
  t-= s.substring(19, 22).toInt() * 3600; // Ajusta p/ UTC
  t+= tz * 3600;                          // Ajusta p/ local
  return t;
}

String dateTimeStr(time_t t, const int8_t tz = 0, const bool flBr = true) {
  // Retorna time_t como "yyyy-mm-dd hh:mm:ss" ou "dd/mm/yyyy hh:mm:ss"
  if (t == 0) {
    return F("N/A");
  }
  t += tz * 3600;
  String sFn;
  if (flBr) {
    // dd/mm/yyyy hh:mm:ss
    sFn = "";
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + '/';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '/' + String(year(t)) + ' ';
  } else {
    // yyyy-mm-dd hh:mm:ss
    sFn = String(year(t)) + '-';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '-';
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + ' ';
  }
  if (hour(t) < 10) {
    sFn += '0';
  }
  sFn += String(hour(t)) + ':';
  if (minute(t) < 10) {
    sFn += '0';
  }
  sFn += String(minute(t)) + ':';
  if (second(t) < 10) {
    sFn += '0';
  }
  sFn += String(second(t));
  return sFn;
}

void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
    Serial.println("Connectando Broker...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
      client.set_callback(callback);
      client.subscribe(MQTT_LED_SET);
      Serial.println("Conectado");
    } else {
      // Falha na conexao
      Serial.println("Falha");
      delay(5000);
    }
  }
}

time_t timeHTTP() {
  // Obtem data/hora via HTTP
  HTTPClient http;
  http.begin(HTTP_TIME_URL);
  int httpCode = http.GET();
  String s = http.getString();
  http.end();
  s.trim();
  while (s[0] > 127) {
    s.remove(0, 1);
  }

  if (httpCode != HTTP_CODE_OK) {
    // Falha na requisicao HTTP
    Serial.println("Falha obtendo Data/Hora");
    setSyncInterval(10);
    return 0;
  }

  // Retorna Data/Hora
  Serial.println("Data/Hora atualizada");
  setSyncInterval(DATETIME_INT);
  return iso8601DateTime(s);
}

// Processa status do LED -------------------------------
char LEDStatus(char c) {
  // Le/Define estado do LED
  String s;
  char st;
  boolean flPublish = true;
  if (c == '0') {
    // LED desligado
    digitalWrite(LED_PIN, LOW);
    s = "desligado";
    st = '0';
  } else if (c == '1') {
    // LED ligado
    digitalWrite(LED_PIN, HIGH);
    s = "ligado";
    st = '1';
  } else if (c == 'x' || c == 'X') {
    // Inverter LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    s = "invertido para ";
    s += (digitalRead(LED_PIN) ? "ligado" : "desligado");
    st = digitalRead(LED_PIN) ? '1' : '0';
  } else {
    // Consulta
    s = "consulta";
    st = digitalRead(LED_PIN) ? '1' : '0';
    flPublish = false;
  }
  if (flPublish) {
    // Atualiza topico MQTT
    client.publish(MQTT::Publish(MQTT_LED_GET, String(st))
                    .set_retain(true));
  }
  // Exibe status
  Serial.println(s);
  return st;
}

// Processa requisoes MQTT ------------------------------
void callback(const MQTT::Publish& pub) {
  // Trata topicos MQTT recebidos
  if (pub.topic() == MQTT_LED_SET) {
    Serial.print("[MQTT] ");
    char c = pub.payload_string()[0];
    LEDStatus(c);
  }
}

// Processa requisoes HTTP ------------------------------
void handleLEDStatus() {
  // Le/Define estado do LED
  Serial.print("[HTTP] ");
  char c = server.arg("set")[0];
  server.send(200, "text/plain", String(LEDStatus(c)));
}

void handleNotFound() {
  // Pagina padrao com Data/Hora e status do LED
  String s =  "<html>"
                "<head>"
                  "<title>ESP32 HTTP Server</title>"
                  "<meta http-equiv=\"Refresh\" content=\"5\">"
                "</head>"
                "<body>"
                  "<h1>ESP32 HTTP Server</h1>"
                  "Data/Hora: " + dateTimeStr(now(), TIME_ZONE) + "<p>"
                  "LED: " + (digitalRead(LED_PIN) ? "Ligado" : "Desligado") +
                "</body>"
              "</html>";
  server.send(200, "text/html", s);
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0));

  // Conecta Wi-Fi
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado, IP "); Serial.println(WiFi.localIP());

  // Servidor HTTP
  server.on("/LEDStatus", handleLEDStatus);
  server.onNotFound(handleNotFound);
  server.begin();

  // Define Data/Hora
  setSyncProvider(timeHTTP);
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

  if (now() != dateTime) {
    // Exibe Data/Hora
    dateTime = now();
    Serial.println(dateTimeStr(dateTime, TIME_ZONE));
  }
}
=======
/********************************************************
 * CANAL INTERNET E COISAS
 * ESP32 - Utilizando HTTP e MQTT
 * 08/2019 - Andre Michelon
 */

// Bibliotecas ------------------------------------------
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <TimeLib.h>

// Wi-Fi ------------------------------------------------
const char* ssid = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";

// Constantes -------------------------------------------
// LED
const byte    LED_PIN             = 2;
// Broker
const char*   MQTT_SERVER         = "internetdascoisas.ddns.net";
// Porta
const int     MQTT_PORT           = 1883;
// Cliente
const char*   MQTT_CLIENT         = "IeCESP32";
// Topico LED Get
const char*   MQTT_LED_GET        = "IeCLED/get";
// Topico LED Set
const char*   MQTT_LED_SET        = "IeCLED/set";
// URL da Data/Hora
const char*   HTTP_TIME_URL       = "https://internetecoisas.com.br/exemplo/iec85.php";
// Fuso horario
const int8_t  TIME_ZONE           = -3;
// Intervalo atualizacao Data/Hora
const int     DATETIME_INT        = 30; // Utilizar 28800 = 8h

// Variaveis globais ------------------------------------
// Data/Hora
time_t        dateTime            = 0;

// Instancias -------------------------------------------
WiFiClientSecure espClient;
PubSubClient client(espClient, MQTT_SERVER, MQTT_PORT);
WebServer server(8221);

// Funcoes Genericas ------------------------------------
time_t iso8601DateTime(String s, const int8_t tz = 0) {
  // Converte String ISO8601 em time_t
  TimeElements te;
  te.Year   = s.substring( 0,  4).toInt() - 1970;
  te.Month  = s.substring( 5,  7).toInt();
  te.Day    = s.substring( 8, 10).toInt();
  te.Hour   = s.substring(11, 13).toInt();
  te.Minute = s.substring(14, 16).toInt();
  te.Second = s.substring(17, 19).toInt();
  time_t t  = makeTime(te);
  t-= s.substring(19, 22).toInt() * 3600; // Ajusta p/ UTC
  t+= tz * 3600;                          // Ajusta p/ local
  return t;
}

String dateTimeStr(time_t t, const int8_t tz = 0, const bool flBr = true) {
  // Retorna time_t como "yyyy-mm-dd hh:mm:ss" ou "dd/mm/yyyy hh:mm:ss"
  if (t == 0) {
    return F("N/A");
  }
  t += tz * 3600;
  String sFn;
  if (flBr) {
    // dd/mm/yyyy hh:mm:ss
    sFn = "";
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + '/';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '/' + String(year(t)) + ' ';
  } else {
    // yyyy-mm-dd hh:mm:ss
    sFn = String(year(t)) + '-';
    if (month(t) < 10) {
      sFn += '0';
    }
    sFn += String(month(t)) + '-';
    if (day(t) < 10) {
      sFn += '0';
    }
    sFn += String(day(t)) + ' ';
  }
  if (hour(t) < 10) {
    sFn += '0';
  }
  sFn += String(hour(t)) + ':';
  if (minute(t) < 10) {
    sFn += '0';
  }
  sFn += String(minute(t)) + ':';
  if (second(t) < 10) {
    sFn += '0';
  }
  sFn += String(second(t));
  return sFn;
}

void reconnect() {
  // Conecta ao Broker
  while (!client.connected()) {
    Serial.println("Connectando Broker...");
    // Conecta
    if (client.connect(MQTT_CLIENT + String(random(1e5)))) {
      client.set_callback(callback);
      client.subscribe(MQTT_LED_SET);
      Serial.println("Conectado");
    } else {
      // Falha na conexao
      Serial.println("Falha");
      delay(5000);
    }
  }
}

time_t timeHTTP() {
  // Obtem data/hora via HTTP
  HTTPClient http;
  http.begin(HTTP_TIME_URL);
  int httpCode = http.GET();
  String s = http.getString();
  http.end();
  s.trim();
  while (s[0] > 127) {
    s.remove(0, 1);
  }

  if (httpCode != HTTP_CODE_OK) {
    // Falha na requisicao HTTP
    Serial.println("Falha obtendo Data/Hora");
    setSyncInterval(10);
    return 0;
  }

  // Retorna Data/Hora
  Serial.println("Data/Hora atualizada");
  setSyncInterval(DATETIME_INT);
  return iso8601DateTime(s);
}

// Processa status do LED -------------------------------
char LEDStatus(char c) {
  // Le/Define estado do LED
  String s;
  char st;
  boolean flPublish = true;
  if (c == '0') {
    // LED desligado
    digitalWrite(LED_PIN, LOW);
    s = "desligado";
    st = '0';
  } else if (c == '1') {
    // LED ligado
    digitalWrite(LED_PIN, HIGH);
    s = "ligado";
    st = '1';
  } else if (c == 'x' || c == 'X') {
    // Inverter LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    s = "invertido para ";
    s += (digitalRead(LED_PIN) ? "ligado" : "desligado");
    st = digitalRead(LED_PIN) ? '1' : '0';
  } else {
    // Consulta
    s = "consulta";
    st = digitalRead(LED_PIN) ? '1' : '0';
    flPublish = false;
  }
  if (flPublish) {
    // Atualiza topico MQTT
    client.publish(MQTT::Publish(MQTT_LED_GET, String(st))
                    .set_retain(true));
  }
  // Exibe status
  Serial.println(s);
  return st;
}

// Processa requisoes MQTT ------------------------------
void callback(const MQTT::Publish& pub) {
  // Trata topicos MQTT recebidos
  if (pub.topic() == MQTT_LED_SET) {
    Serial.print("[MQTT] ");
    char c = pub.payload_string()[0];
    LEDStatus(c);
  }
}

// Processa requisoes HTTP ------------------------------
void handleLEDStatus() {
  // Le/Define estado do LED
  Serial.print("[HTTP] ");
  char c = server.arg("set")[0];
  server.send(200, "text/plain", String(LEDStatus(c)));
}

void handleNotFound() {
  // Pagina padrao com Data/Hora e status do LED
  String s =  "<html>"
                "<head>"
                  "<title>ESP32 HTTP Server</title>"
                  "<meta http-equiv=\"Refresh\" content=\"5\">"
                "</head>"
                "<body>"
                  "<h1>ESP32 HTTP Server</h1>"
                  "Data/Hora: " + dateTimeStr(now(), TIME_ZONE) + "<p>"
                  "LED: " + (digitalRead(LED_PIN) ? "Ligado" : "Desligado") +
                "</body>"
              "</html>";
  server.send(200, "text/html", s);
}

// Setup ------------------------------------------------
void setup() {
  // Incializa
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  randomSeed(analogRead(0));

  // Conecta Wi-Fi
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado, IP "); Serial.println(WiFi.localIP());

  // Servidor HTTP
  server.on("/LEDStatus", handleLEDStatus);
  server.onNotFound(handleNotFound);
  server.begin();

  // Define Data/Hora
  setSyncProvider(timeHTTP);
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

  if (now() != dateTime) {
    // Exibe Data/Hora
    dateTime = now();
    Serial.println(dateTimeStr(dateTime, TIME_ZONE));
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
