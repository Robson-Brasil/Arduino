<<<<<<< HEAD
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <FS.h>
#include <SPIFFS.h>

//endereço I2C do MCP23017
#define MCP_ADDRESS 0x20
//ENDEREÇOS DE REGISTRADORES
#define GPA 0x12 // DATA PORT REGISTER A
#define GPB 0x13 // DATA PORT REGISTER B 
#define IODIRA 0x00 // I/O DIRECTION REGISTER A
#define IODIRB 0x01 // I/O DIRECTION REGISTER B
#define PINS_COUNT 16 //Quantidade total de pinos
#define DATA_PATH "/pin_data.bin" //Arquivo onde serão salvos os status dos pinos
#define DHTPIN 15 //Pino one está o DHT22

const char *ssid = "MOTOROLA-A7CE9";
const char *password = "3104e43ece65b0885fa9";
const char *ip = "192.168.0.2";

//Criamos um server na porta padrão o http
WebServer server(80);

//Objeto que faz a leitura da temperatura e umidade
SimpleDHT22 dht;

//Variáveis para guardar os valores de temperatura e umidade lidos
float temperature = 0;
float humidity = 0;

//Guarda o estado atual das duas portas do MCP23017 (8 bits cada)
uint8_t currentValueGPA = 0;
uint8_t currentValueGPB = 0;

//faz o controle do temporizador do watchdog (interrupção por tempo)
hw_timer_t *timer = NULL; 

void setup()
{
  Serial.begin(115200);

  //Inicializa os valores dos pinos do MCP23017
  setupPins();

  //Tenta inicializar SPIFFS
  if(SPIFFS.begin(true))
  {
    loadPinStatus(); 
  }
  else
  {
    //Se não conseguiu inicializar
    Serial.println("SPIFFS Mount Failed");
  }

  //inicializa WiFi
  setupWiFi();

  //Sempre que recebermos uma requisição na raiz do webserver
  //a função handleRoot será executada
  server.on("/", handleRoot);

  //Se recebermos uma requisição em uma rota que nao existe
  server.onNotFound(handleNotFound);

  //Inicializa o server
  server.begin();

  //Inicializa o watchdog
  setupWatchdog();
}

void setupPins()
{
  //Inicializa o Wire nos pinos SDA e SCL padrões do ESP32
  Wire.begin();
  //Velocidade de comunicação
  Wire.setClock(200000);

  //Configura todos os pinos das duas portas do MCP23017 como saída
  configurePort(IODIRA, OUTPUT);
  configurePort(IODIRB, OUTPUT);
}

//Função para recuperar o estado atual dos pinos no 
//arquivo para que estes permaneçam após um eventual reboot
void loadPinStatus()
{
  //Abre o arquivo para leitura
  File file = SPIFFS.open(DATA_PATH, FILE_READ);
  
  //Se arquivo não existe
  if(!file)
  {
    //Na primeira vez o arquivo ainda não foi criado
    Serial.println("Failed to open file for reading");
    //Coloca todos os pinos das duas portas do MCP23017 em LOW
    writeBlockData(GPA, B00000000);
    writeBlockData(GPB, B00000000);
    return;
  }

  //Faz a leitura dos valores
  file.read(&currentValueGPA, 1);
  file.read(&currentValueGPB, 1);
  //fecha o arquivo
  file.close();

  //Envia os valores para o MCP23017
  writeBlockData(GPA, currentValueGPA);
  writeBlockData(GPB, currentValueGPB);
}

void setupWiFi()
{
  //Coloca como modo station
  WiFi.mode(WIFI_STA);
  //Conecta à rede
  WiFi.begin(ssid, password);
  Serial.println("");

  //Enquanto não conectar
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //Se chegou aqui está conectado
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  //Configura o IP
  IPAddress ipAddress;
  ipAddress.fromString(ip);
  WiFi.config(ipAddress, WiFi.gatewayIP(), WiFi.subnetMask());

  //Exibe o endereço de IP para abrir no navegador
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule(){
  ets_printf("(watchdog) reboot\n");
  esp_restart_noos(); //reinicia o chip
}
 
//função que configura o temporizador
void setupWatchdog()
{
  timer = timerBegin(0, 80, true); //timerID 0, div 80
  //timer, callback, interrupção de borda
  timerAttachInterrupt(timer, &resetModule, true);
  //timer, tempo (us), repetição
  timerAlarmWrite(timer, 5000000, true);
  timerAlarmEnable(timer); //habilita a interrupção //enable interrupt
}

void loop()
{
  //reseta o temporizador (alimenta o watchdog)
  timerWrite(timer, 0); 
  //Verifica se existe alguma requisição
  server.handleClient();
}

void handleRoot()
{
  //Se recebeu argumentos na requisição
  if(server.args() > 0)
  {
    //Executa a ação (on ou off) no pino do argumento
    execute(server.argName(0), server.arg(0));
  }

  //Faz a leitura da temperatura e umidade
  readDHT();
  
  //Gera o html e o envia
  String html = "<html>";
  html.concat(head());
  html.concat(body()); 
  html.concat("</html>");  
  server.send(200, "text/html; charset=UTF-8", html);

  //Salva o status dos pinos para voltar assim no próximo reboot
  savePinStatus();
}

//Função para salvar o estado atual dos pinos em 
//arquivo para que estes permaneçam após um eventual reboot
void savePinStatus()
{
  //Abre o arquivo para escrita
  File file = SPIFFS.open(DATA_PATH, FILE_WRITE);
  
  //Se não conseguiu abrir/criar o arquivo
  if(!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  //Escreve os valores dos pinos no começo do arquivo
  file.seek(0);
  file.write(&currentValueGPA, 1);
  file.write(&currentValueGPB, 1);
  //Fecha o arquivo
  file.close();
}

void handleNotFound()
{
  //Envia para o navegador a informação que a rota não foi encontrada
  server.send(404, "text/plain", "Not Found");
}

//Executada a ação junto ao valor (número do relê)
void execute(String action, String value)
{
  //Se é uma das duas ações que esperamos
  if(action == "on" || action == "off")
  {
    //Os relês são numerados a partir do 1, mas o array começa do 0
    //então tiramos 1
    int index = value.toInt() - 1;
    int status = action == "on" ? HIGH : LOW;
    digitalWriteMCP(index, status);
  }
}

//Retorna o cabeçalho da página com a informação do tempo
//para atualizar a página sozinho e a aparência
String head()
{
    return (F("<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<meta http-equiv='refresh' content='10;URL=/'>" //refresh a cada 10 segundos
    "<style>"
        "body{"
            "text-align: center;"
            "font-family: sans-serif;"
            "font-size: 14px;"
        "}"
        "p{"
            "color:#555;"
            "font-size: 12px;"
        "}"
        ".button{"
            "outline: none;"
            "display: block;"
            "border: 1px solid #555;"
            "border-radius:18px;"
            "width: 150px;"
            "height: 30px;"
            "margin: 10px;"
            "margin-left: auto;"
            "margin-right: auto;"
            "cursor: pointer;"
        "}"
        ".button_off{"
            "background-color:#FFF;"
            "color: #555;"
        "}"
        ".button_on{"
            "background-color:#2C5;"
            "color: #fff;"
        "}"
    "</style>"
"</head>"));
}

//Exibe os dados dos sensores e cria os botões
String body()
{
  String b = "<body>"
    "<p>Temperature: " + String(temperature) + " °C</p>"
    "<p>Humidity: " + String(humidity) + "%</p>";

    //Cria um botão para cada pino que possui um relê
    for(int i=0; i<PINS_COUNT; i++)
    {
        b.concat(button(i));
    }

    b.concat("</body>");

    return b;
}

//Cria um botão com a aparência e ação correspondente ao estado atual do relê
String button(int number)
{
  String label = String(number + 1);
  String className = "button ";
  className += getPinStatus(number) == HIGH ? "button_on" : "button_off";
  String action = getPinStatus(number) == HIGH ? "off" : "on";
  return "<button class=\"" + className + "\"onclick=\"location.href='?" + action + "=" + label + "'\">" + label + "</button>";
}

uint8_t getPinStatus(int pin)
{
  uint8_t v;

  //de 0 a 7 porta A, de 8 a 15 porta B
  if(pin < 8)
  {
    v = currentValueGPA;
  }
  else
  {
    v = currentValueGPB;
    pin -= 8;
  }

  return !!(v & (1 << pin));
}

//Configura o modo dos pinos das portas (GPA ou GPB)
//como parametro passamos:
//  port: GPA ou GPB
//  type:
//    INPUT para todos os pinos da porta trabalharem como entrada
//    OUTPUT para todos os pinos da porta trabalharem como saída
void configurePort(uint8_t port, uint8_t type)
{
  if(type == INPUT)
  {
    writeBlockData(port, 0xFF);
  }
  else if(type == OUTPUT)
  {
    writeBlockData(port, 0x00);
  }
}

//muda o estado de um pino desejado
void digitalWriteMCP(int pin, int value)
{
  uint8_t port;
  uint8_t v;

  //de 0 a 7 porta A, de 8 a 15 porta B
  if(pin < 8){
    port = GPA;
    v = currentValueGPA;
  }else{
    port = GPB;
    v = currentValueGPB;
    pin -= 8;
  }
    
  if (value == LOW){
    v &= ~(B00000001 << (pin)); // muda o pino para LOW
  }
  else if (value == HIGH){
    v |= (B00000001 << (pin)); // muda o pino para HIGH
  }

  //Salva os valores dos bits da porta correspondente
  if(port == GPA){
    currentValueGPA = v;
  }else{
    currentValueGPB = v;
  }

  //envia os dados para o MCP
  writeBlockData(port, v);
}

//envia dados para o MCP23017 através do barramento i2c
void writeBlockData(uint8_t port, uint8_t data)
{
  Wire.beginTransmission(MCP_ADDRESS);
  Wire.write(port);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

//Faz a leitura da temperatura e umidade
void readDHT()
{
  float t, h;
  int status = dht.read2(DHTPIN, &t, &h, NULL);

  //Apenas altera as variáveis se a leitura foi bem sucedida
  if (status == SimpleDHTErrSuccess)
  {
    temperature = t;
    humidity = h;
  }
}
=======
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <FS.h>
#include <SPIFFS.h>

//endereço I2C do MCP23017
#define MCP_ADDRESS 0x20
//ENDEREÇOS DE REGISTRADORES
#define GPA 0x12 // DATA PORT REGISTER A
#define GPB 0x13 // DATA PORT REGISTER B 
#define IODIRA 0x00 // I/O DIRECTION REGISTER A
#define IODIRB 0x01 // I/O DIRECTION REGISTER B
#define PINS_COUNT 16 //Quantidade total de pinos
#define DATA_PATH "/pin_data.bin" //Arquivo onde serão salvos os status dos pinos
#define DHTPIN 15 //Pino one está o DHT22

const char *ssid = "MOTOROLA-A7CE9";
const char *password = "3104e43ece65b0885fa9";
const char *ip = "192.168.0.2";

//Criamos um server na porta padrão o http
WebServer server(80);

//Objeto que faz a leitura da temperatura e umidade
SimpleDHT22 dht;

//Variáveis para guardar os valores de temperatura e umidade lidos
float temperature = 0;
float humidity = 0;

//Guarda o estado atual das duas portas do MCP23017 (8 bits cada)
uint8_t currentValueGPA = 0;
uint8_t currentValueGPB = 0;

//faz o controle do temporizador do watchdog (interrupção por tempo)
hw_timer_t *timer = NULL; 

void setup()
{
  Serial.begin(115200);

  //Inicializa os valores dos pinos do MCP23017
  setupPins();

  //Tenta inicializar SPIFFS
  if(SPIFFS.begin(true))
  {
    loadPinStatus(); 
  }
  else
  {
    //Se não conseguiu inicializar
    Serial.println("SPIFFS Mount Failed");
  }

  //inicializa WiFi
  setupWiFi();

  //Sempre que recebermos uma requisição na raiz do webserver
  //a função handleRoot será executada
  server.on("/", handleRoot);

  //Se recebermos uma requisição em uma rota que nao existe
  server.onNotFound(handleNotFound);

  //Inicializa o server
  server.begin();

  //Inicializa o watchdog
  setupWatchdog();
}

void setupPins()
{
  //Inicializa o Wire nos pinos SDA e SCL padrões do ESP32
  Wire.begin();
  //Velocidade de comunicação
  Wire.setClock(200000);

  //Configura todos os pinos das duas portas do MCP23017 como saída
  configurePort(IODIRA, OUTPUT);
  configurePort(IODIRB, OUTPUT);
}

//Função para recuperar o estado atual dos pinos no 
//arquivo para que estes permaneçam após um eventual reboot
void loadPinStatus()
{
  //Abre o arquivo para leitura
  File file = SPIFFS.open(DATA_PATH, FILE_READ);
  
  //Se arquivo não existe
  if(!file)
  {
    //Na primeira vez o arquivo ainda não foi criado
    Serial.println("Failed to open file for reading");
    //Coloca todos os pinos das duas portas do MCP23017 em LOW
    writeBlockData(GPA, B00000000);
    writeBlockData(GPB, B00000000);
    return;
  }

  //Faz a leitura dos valores
  file.read(&currentValueGPA, 1);
  file.read(&currentValueGPB, 1);
  //fecha o arquivo
  file.close();

  //Envia os valores para o MCP23017
  writeBlockData(GPA, currentValueGPA);
  writeBlockData(GPB, currentValueGPB);
}

void setupWiFi()
{
  //Coloca como modo station
  WiFi.mode(WIFI_STA);
  //Conecta à rede
  WiFi.begin(ssid, password);
  Serial.println("");

  //Enquanto não conectar
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //Se chegou aqui está conectado
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  //Configura o IP
  IPAddress ipAddress;
  ipAddress.fromString(ip);
  WiFi.config(ipAddress, WiFi.gatewayIP(), WiFi.subnetMask());

  //Exibe o endereço de IP para abrir no navegador
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule(){
  ets_printf("(watchdog) reboot\n");
  esp_restart_noos(); //reinicia o chip
}
 
//função que configura o temporizador
void setupWatchdog()
{
  timer = timerBegin(0, 80, true); //timerID 0, div 80
  //timer, callback, interrupção de borda
  timerAttachInterrupt(timer, &resetModule, true);
  //timer, tempo (us), repetição
  timerAlarmWrite(timer, 5000000, true);
  timerAlarmEnable(timer); //habilita a interrupção //enable interrupt
}

void loop()
{
  //reseta o temporizador (alimenta o watchdog)
  timerWrite(timer, 0); 
  //Verifica se existe alguma requisição
  server.handleClient();
}

void handleRoot()
{
  //Se recebeu argumentos na requisição
  if(server.args() > 0)
  {
    //Executa a ação (on ou off) no pino do argumento
    execute(server.argName(0), server.arg(0));
  }

  //Faz a leitura da temperatura e umidade
  readDHT();
  
  //Gera o html e o envia
  String html = "<html>";
  html.concat(head());
  html.concat(body()); 
  html.concat("</html>");  
  server.send(200, "text/html; charset=UTF-8", html);

  //Salva o status dos pinos para voltar assim no próximo reboot
  savePinStatus();
}

//Função para salvar o estado atual dos pinos em 
//arquivo para que estes permaneçam após um eventual reboot
void savePinStatus()
{
  //Abre o arquivo para escrita
  File file = SPIFFS.open(DATA_PATH, FILE_WRITE);
  
  //Se não conseguiu abrir/criar o arquivo
  if(!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  //Escreve os valores dos pinos no começo do arquivo
  file.seek(0);
  file.write(&currentValueGPA, 1);
  file.write(&currentValueGPB, 1);
  //Fecha o arquivo
  file.close();
}

void handleNotFound()
{
  //Envia para o navegador a informação que a rota não foi encontrada
  server.send(404, "text/plain", "Not Found");
}

//Executada a ação junto ao valor (número do relê)
void execute(String action, String value)
{
  //Se é uma das duas ações que esperamos
  if(action == "on" || action == "off")
  {
    //Os relês são numerados a partir do 1, mas o array começa do 0
    //então tiramos 1
    int index = value.toInt() - 1;
    int status = action == "on" ? HIGH : LOW;
    digitalWriteMCP(index, status);
  }
}

//Retorna o cabeçalho da página com a informação do tempo
//para atualizar a página sozinho e a aparência
String head()
{
    return (F("<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<meta http-equiv='refresh' content='10;URL=/'>" //refresh a cada 10 segundos
    "<style>"
        "body{"
            "text-align: center;"
            "font-family: sans-serif;"
            "font-size: 14px;"
        "}"
        "p{"
            "color:#555;"
            "font-size: 12px;"
        "}"
        ".button{"
            "outline: none;"
            "display: block;"
            "border: 1px solid #555;"
            "border-radius:18px;"
            "width: 150px;"
            "height: 30px;"
            "margin: 10px;"
            "margin-left: auto;"
            "margin-right: auto;"
            "cursor: pointer;"
        "}"
        ".button_off{"
            "background-color:#FFF;"
            "color: #555;"
        "}"
        ".button_on{"
            "background-color:#2C5;"
            "color: #fff;"
        "}"
    "</style>"
"</head>"));
}

//Exibe os dados dos sensores e cria os botões
String body()
{
  String b = "<body>"
    "<p>Temperature: " + String(temperature) + " °C</p>"
    "<p>Humidity: " + String(humidity) + "%</p>";

    //Cria um botão para cada pino que possui um relê
    for(int i=0; i<PINS_COUNT; i++)
    {
        b.concat(button(i));
    }

    b.concat("</body>");

    return b;
}

//Cria um botão com a aparência e ação correspondente ao estado atual do relê
String button(int number)
{
  String label = String(number + 1);
  String className = "button ";
  className += getPinStatus(number) == HIGH ? "button_on" : "button_off";
  String action = getPinStatus(number) == HIGH ? "off" : "on";
  return "<button class=\"" + className + "\"onclick=\"location.href='?" + action + "=" + label + "'\">" + label + "</button>";
}

uint8_t getPinStatus(int pin)
{
  uint8_t v;

  //de 0 a 7 porta A, de 8 a 15 porta B
  if(pin < 8)
  {
    v = currentValueGPA;
  }
  else
  {
    v = currentValueGPB;
    pin -= 8;
  }

  return !!(v & (1 << pin));
}

//Configura o modo dos pinos das portas (GPA ou GPB)
//como parametro passamos:
//  port: GPA ou GPB
//  type:
//    INPUT para todos os pinos da porta trabalharem como entrada
//    OUTPUT para todos os pinos da porta trabalharem como saída
void configurePort(uint8_t port, uint8_t type)
{
  if(type == INPUT)
  {
    writeBlockData(port, 0xFF);
  }
  else if(type == OUTPUT)
  {
    writeBlockData(port, 0x00);
  }
}

//muda o estado de um pino desejado
void digitalWriteMCP(int pin, int value)
{
  uint8_t port;
  uint8_t v;

  //de 0 a 7 porta A, de 8 a 15 porta B
  if(pin < 8){
    port = GPA;
    v = currentValueGPA;
  }else{
    port = GPB;
    v = currentValueGPB;
    pin -= 8;
  }
    
  if (value == LOW){
    v &= ~(B00000001 << (pin)); // muda o pino para LOW
  }
  else if (value == HIGH){
    v |= (B00000001 << (pin)); // muda o pino para HIGH
  }

  //Salva os valores dos bits da porta correspondente
  if(port == GPA){
    currentValueGPA = v;
  }else{
    currentValueGPB = v;
  }

  //envia os dados para o MCP
  writeBlockData(port, v);
}

//envia dados para o MCP23017 através do barramento i2c
void writeBlockData(uint8_t port, uint8_t data)
{
  Wire.beginTransmission(MCP_ADDRESS);
  Wire.write(port);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

//Faz a leitura da temperatura e umidade
void readDHT()
{
  float t, h;
  int status = dht.read2(DHTPIN, &t, &h, NULL);

  //Apenas altera as variáveis se a leitura foi bem sucedida
  if (status == SimpleDHTErrSuccess)
  {
    temperature = t;
    humidity = h;
  }
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
