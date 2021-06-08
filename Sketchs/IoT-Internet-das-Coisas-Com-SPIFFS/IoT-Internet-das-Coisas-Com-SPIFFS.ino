<<<<<<< HEAD

// Bibliotecas Usadas
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Configuração do Wi-Fi
const char* ssid = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";
IPAddress staticIP(192, 168, 0, 20);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);
const char* http_username = "Robson Brasil";
const char* http_password = "@Lobo#Alfa@";
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Configuração da Porta Usada Pelo AsyncWebServer
AsyncWebServer server(8221);

// Configuração HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <html lang="pt-br"></html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><style data-merge-styles="true"></style>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 100px; height: 50px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #00FF1E; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 32px; width: 32px; left: 8px; bottom: 8px; background-color: #000000; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #FF1000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <center><h1>Automação Residencial</h1> 
  <center><h1>Com ESP32 - IoT</h1>
 <!DOCTYPE HTML><html>
<head>
 <style type="text/css">
.tg  {border-collapse:collapse;border-color:#9ABAD9;border-spacing:0;}
.tg td{background-color:#EBF5FF;border-color:#9ABAD9;border-style:solid;border-width:1px;color:#444;
  font-family:Arial, sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{background-color:#409cff;border-color:#9ABAD9;border-style:solid;border-width:1px;color:#fff;
  font-family:Arial, sans-serif;font-size:14px;font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-fbeb{background-color:#9b9b9b;border-color:#9b9b9b;color:#333333;text-align:left;vertical-align:top}
.tg .tg-i91a{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-0pky{border-color:inherit;text-align:center;vertical-align:center}
.tg .tg-i91b{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-1pky{border-color:inherit;text-align:center;vertical-align:center}
.tg .tg-i91c{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-2pky{border-color:inherit;text-align:center;vertical-align:center}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-fbeb"></th>
    <th class="tg-c3ow" colspan="2">Interruptor 1</th>
    <th class="tg-c3ow" colspan="2">Interruptor 2</th>
    <th class="tg-c3ow" colspan="2">Interruptor 3</th>
    <th class="tg-c3ow" colspan="2">Interruptor 4</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-i91a"><h4>Conjunto 1</h4></td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER1%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER2%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER3%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER4%</td>
    </tr>
    <tr> 
    <td class="tg-i91b"><h4>Conjunto 2</h4></td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER5%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER6%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER7%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER8%</td>
     </tr>
     <tr>
    <td class="tg-i91c"><h4>Conjunto 3</h4></td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER9%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER10%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER11%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER12%</td>
     </tr>
     <tr>
    
</tbody>
</table>
  
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</head>
</html>
)rawliteral";

// Configuração dos Botões Usados
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER1"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 1</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"21\" " + outputState(21) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER2"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER3"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 3</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(14) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER4"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"27\" " + outputState(27) + "><span class=\"slider\"></span></label>";
     return buttons;
  }

  if(var == "BUTTONPLACEHOLDER5"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"26\" " + outputState(26) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER6"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 6</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"25\" " + outputState(25) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER7"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 7</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER8"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 8</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
 if(var == "BUTTONPLACEHOLDER9"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 9</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"19\" " + outputState(19) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER10"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 10</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   
  return String();
}
String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
    // Porta Serial para Debugação
    Serial.begin(115200);
    SPIFFS.begin();
    //Envia o arquivo html quando entrar quando fizerem uma requisição na raiz do servidor
    server.serveStatic("/", SPIFFS, "/data/IoT-Internet-das-Coisas.html");
 
  pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
  pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
  pinMode(14, OUTPUT);
    digitalWrite(14, LOW);
  pinMode(27, OUTPUT);
    digitalWrite(27, LOW);

  pinMode(26, OUTPUT);
    digitalWrite(26, LOW);
  pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
  pinMode(33, OUTPUT);
    digitalWrite(33, LOW);
  pinMode(32, OUTPUT);
    digitalWrite(32, LOW);

  pinMode(19, OUTPUT);
    digitalWrite(19, LOW);
  pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
  pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  
  // Conectado ao Wi-Fi
  if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
    Serial.println("Configuração falhou.");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  
  // Print do IP Local do ESP32
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start do Servidor
  server.begin();
  
}

void loop() {

}
=======

// Bibliotecas Usadas
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Configuração do Wi-Fi
const char* ssid = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";
IPAddress staticIP(192, 168, 0, 20);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);
const char* http_username = "Robson Brasil";
const char* http_password = "@Lobo#Alfa@";
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Configuração da Porta Usada Pelo AsyncWebServer
AsyncWebServer server(8221);

// Configuração HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <html lang="pt-br"></html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><style data-merge-styles="true"></style>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 100px; height: 50px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #00FF1E; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 32px; width: 32px; left: 8px; bottom: 8px; background-color: #000000; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #FF1000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <center><h1>Automação Residencial</h1> 
  <center><h1>Com ESP32 - IoT</h1>
 <!DOCTYPE HTML><html>
<head>
 <style type="text/css">
.tg  {border-collapse:collapse;border-color:#9ABAD9;border-spacing:0;}
.tg td{background-color:#EBF5FF;border-color:#9ABAD9;border-style:solid;border-width:1px;color:#444;
  font-family:Arial, sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{background-color:#409cff;border-color:#9ABAD9;border-style:solid;border-width:1px;color:#fff;
  font-family:Arial, sans-serif;font-size:14px;font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-fbeb{background-color:#9b9b9b;border-color:#9b9b9b;color:#333333;text-align:left;vertical-align:top}
.tg .tg-i91a{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-0pky{border-color:inherit;text-align:center;vertical-align:center}
.tg .tg-i91b{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-1pky{border-color:inherit;text-align:center;vertical-align:center}
.tg .tg-i91c{border-color:inherit;color:#333333;text-align:center;vertical-align:center}
.tg .tg-2pky{border-color:inherit;text-align:center;vertical-align:center}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-fbeb"></th>
    <th class="tg-c3ow" colspan="2">Interruptor 1</th>
    <th class="tg-c3ow" colspan="2">Interruptor 2</th>
    <th class="tg-c3ow" colspan="2">Interruptor 3</th>
    <th class="tg-c3ow" colspan="2">Interruptor 4</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-i91a"><h4>Conjunto 1</h4></td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER1%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER2%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER3%</td>
    <td class="tg-0pky" colspan="2">%BUTTONPLACEHOLDER4%</td>
    </tr>
    <tr> 
    <td class="tg-i91b"><h4>Conjunto 2</h4></td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER5%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER6%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER7%</td>
    <td class="tg-1pky" colspan="2">%BUTTONPLACEHOLDER8%</td>
     </tr>
     <tr>
    <td class="tg-i91c"><h4>Conjunto 3</h4></td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER9%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER10%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER11%</td>
    <td class="tg-2pky" colspan="2">%BUTTONPLACEHOLDER12%</td>
     </tr>
     <tr>
    
</tbody>
</table>
  
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</head>
</html>
)rawliteral";

// Configuração dos Botões Usados
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER1"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 1</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"21\" " + outputState(21) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER2"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER3"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 3</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(14) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER4"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"27\" " + outputState(27) + "><span class=\"slider\"></span></label>";
     return buttons;
  }

  if(var == "BUTTONPLACEHOLDER5"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"26\" " + outputState(26) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER6"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 6</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"25\" " + outputState(25) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER7"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 7</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   if(var == "BUTTONPLACEHOLDER8"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 8</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
 if(var == "BUTTONPLACEHOLDER9"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 9</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"19\" " + outputState(19) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
   if(var == "BUTTONPLACEHOLDER10"){
    String buttons = "";
    buttons += "<h4>Ativar Interruptor 10</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
     return buttons;
  }
   
  return String();
}
String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
    // Porta Serial para Debugação
    Serial.begin(115200);
    SPIFFS.begin();
    //Envia o arquivo html quando entrar quando fizerem uma requisição na raiz do servidor
    server.serveStatic("/", SPIFFS, "/data/IoT-Internet-das-Coisas.html");
 
  pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
  pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
  pinMode(14, OUTPUT);
    digitalWrite(14, LOW);
  pinMode(27, OUTPUT);
    digitalWrite(27, LOW);

  pinMode(26, OUTPUT);
    digitalWrite(26, LOW);
  pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
  pinMode(33, OUTPUT);
    digitalWrite(33, LOW);
  pinMode(32, OUTPUT);
    digitalWrite(32, LOW);

  pinMode(19, OUTPUT);
    digitalWrite(19, LOW);
  pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
  pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  
  // Conectado ao Wi-Fi
  if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
    Serial.println("Configuração falhou.");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi..");
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  
  // Print do IP Local do ESP32
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start do Servidor
  server.begin();
  
}

void loop() {

}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
