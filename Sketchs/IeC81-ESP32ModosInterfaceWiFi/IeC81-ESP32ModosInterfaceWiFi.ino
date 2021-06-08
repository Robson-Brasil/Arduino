<<<<<<< HEAD
/********************************************************
 * CANAL INTERNET E COISAS
 * ESP32 - Modos de Operacao da Interface WiFi
 * 08/2019 - Andre Michelon
 */

// Bibliotecas
#include <WiFi.h>

// Configuracao Rede WiFi
const char *ssid     = "home2.4";
const char *password = "Home31415";

void setup(){
  // Inicializa Serial
  Serial.begin(115200);
  Serial.println("\nIniciando!");

  // Define ID de Rede do Dispositivo
  WiFi.setHostname("ESP32");
}

void loop() {
  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Station
  Serial.println("\nWiFi STA ------------");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  hold();

  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Access Point
  Serial.println("\nWiFi AP ------------");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_AP", "esp012345678");
  hold();

  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Access Point + Station
  Serial.println("\nWiFi AP_STA ------------");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32_AP", "esp012345678");
  WiFi.begin(ssid, password);
  hold();
}

void hold() {
  // Aguarda tempo e exibe status
  unsigned long t;
  String s;

  for (int8_t i = 30; i > 0; i--) {
    switch (WiFi.status()) {
      case WL_NO_SHIELD:
        s = "WL_NO_SHIELD      ";
        break;
      case WL_IDLE_STATUS:
        s = "WL_IDLE_STATUS    ";
        break;
      case WL_NO_SSID_AVAIL:
        s = "WL_NO_SSID_AVAIL  ";
        break;
      case WL_SCAN_COMPLETED:
        s = "WL_SCAN_COMPLETED ";
        break;
      case WL_CONNECTED:
        s = "WL_CONNECTED      ";
        break;
      case WL_CONNECT_FAILED:
        s = "WL_CONNECT_FAILED ";
        break;
      case WL_CONNECTION_LOST:
        s = "WL_CONNECTION_LOST";
        break;
      case WL_DISCONNECTED:
        s = "WL_DISCONNECTED   ";
        break;
      default:
        s = "N/D         " + String(WiFi.status());
        break;
    }
    Serial.print(String(i) + "\t" + s +
                  "\t SSID: " + WiFi.SSID() +
                  "\tCh: " + String(WiFi.channel()) +
                  "\tRSSI: " + String(WiFi.RSSI()) +
                  "\t IP: ");
    Serial.println(WiFi.localIP());
    t = millis() + 1000;
    while (millis() < t) {
      yield();
    }
  }
}
=======
/********************************************************
 * CANAL INTERNET E COISAS
 * ESP32 - Modos de Operacao da Interface WiFi
 * 08/2019 - Andre Michelon
 */

// Bibliotecas
#include <WiFi.h>

// Configuracao Rede WiFi
const char *ssid     = "home2.4";
const char *password = "Home31415";

void setup(){
  // Inicializa Serial
  Serial.begin(115200);
  Serial.println("\nIniciando!");

  // Define ID de Rede do Dispositivo
  WiFi.setHostname("ESP32");
}

void loop() {
  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Station
  Serial.println("\nWiFi STA ------------");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  hold();

  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Access Point
  Serial.println("\nWiFi AP ------------");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_AP", "esp012345678");
  hold();

  // WiFi desligado
  Serial.println("\nWiFi Off ------------");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  hold();

  // WiFi modo Access Point + Station
  Serial.println("\nWiFi AP_STA ------------");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32_AP", "esp012345678");
  WiFi.begin(ssid, password);
  hold();
}

void hold() {
  // Aguarda tempo e exibe status
  unsigned long t;
  String s;

  for (int8_t i = 30; i > 0; i--) {
    switch (WiFi.status()) {
      case WL_NO_SHIELD:
        s = "WL_NO_SHIELD      ";
        break;
      case WL_IDLE_STATUS:
        s = "WL_IDLE_STATUS    ";
        break;
      case WL_NO_SSID_AVAIL:
        s = "WL_NO_SSID_AVAIL  ";
        break;
      case WL_SCAN_COMPLETED:
        s = "WL_SCAN_COMPLETED ";
        break;
      case WL_CONNECTED:
        s = "WL_CONNECTED      ";
        break;
      case WL_CONNECT_FAILED:
        s = "WL_CONNECT_FAILED ";
        break;
      case WL_CONNECTION_LOST:
        s = "WL_CONNECTION_LOST";
        break;
      case WL_DISCONNECTED:
        s = "WL_DISCONNECTED   ";
        break;
      default:
        s = "N/D         " + String(WiFi.status());
        break;
    }
    Serial.print(String(i) + "\t" + s +
                  "\t SSID: " + WiFi.SSID() +
                  "\tCh: " + String(WiFi.channel()) +
                  "\tRSSI: " + String(WiFi.RSSI()) +
                  "\t IP: ");
    Serial.println(WiFi.localIP());
    t = millis() + 1000;
    while (millis() < t) {
      yield();
    }
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
