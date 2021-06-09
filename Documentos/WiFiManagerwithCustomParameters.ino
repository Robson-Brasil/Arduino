//include <FS.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson


//define your default values here, if there are different values in config.json, they are overwritten.
char Voltage[40];
char Frequency[6];
char Duty_Cycle[34];

//flag for saving data
bool shouldSaveConfig = true;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //read configuration from FS json
  Serial.println("mounting FS...");
  SPIFFS.begin (true);

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(Voltage, json["Voltage"]);
          strcpy(Frequency, json["Frequency"]);
          strcpy(Duty_Cycle, json["Duty_Cycle"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_Voltage("Voltage", "Voltage", Voltage, 40);
  WiFiManagerParameter custom_Frequency("Frequency", "Frequency", Frequency, 6);
  WiFiManagerParameter custom_Duty_Cycle("Duty Cycle", "Duty Cycle", Duty_Cycle, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //add all your parameters here
  wifiManager.addParameter(&custom_Voltage);
  wifiManager.addParameter(&custom_Frequency);
  wifiManager.addParameter(&custom_Duty_Cycle);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("Proto G // ESP32", "G42")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(Voltage, custom_Voltage.getValue());
  strcpy(Frequency, custom_Frequency.getValue());
  strcpy(Duty_Cycle, custom_Duty_Cycle.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["Voltage"] = Voltage;
    json["Frequency"] = Frequency;
    json["Duty_Cycle"] = Duty_Cycle;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.print("Voltage: ");
  Serial.print(Voltage);
  Serial.println("V");
  Serial.println();
  Serial.print("Frequency: ");
  Serial.print(Frequency);
  Serial.println("Hz");
  Serial.println();
  Serial.print("Duty Cycle: ");
  Serial.print(Duty_Cycle);
  Serial.println("%");

  //WiFi.disconnect(true); //erases store credentially
  //SPIFFS.format();  //erases stored values
  Serial.println("Done");

}

void loop() {
///////////////////////////////////////////////////////////////////////

//Uncomment these lines of code if you want to reset the device 
//It could be linked to a physical reset button on the device and set
//to trigger the next 3 lines of code.
  //WiFi.disconnect(true); //erases store credentially
  //SPIFFS.format();  //erases stored values
  //ESP.restart();
///////////////////////////////////////////////////////////////////////


}
