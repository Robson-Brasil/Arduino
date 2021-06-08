<<<<<<< HEAD


#include <Arduino.h> //Lib Arduino
#include <WiFi.h> // Lib WiFi
#include <Wire.h>  // Necessário apenas para o Arduino 1.6.5 e posterior
#include <SPI.h> // Lib de comunicação SPI
#include <vector> // Lib com as funções de vetor (vector)
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h> //Lib Firebase
#define FIREBASE_HOST "https://automacao-residencial-83a57.firebaseio.com" //databaseURL fornecido pelo Firebase para conexão
#define FIREBASE_AUTH ""
#include "esp_task_wdt.h"
const char* ssid = "MOTOROLA-A7CE9"; // Coloque o nome da sua rede wifi aqui
const char* password = "3104e43ece65b0885fa9"; // Coloque a sua senha wifi aqui

const IPAddress IP = IPAddress(192,168,0,15); // IP fixo que o ESP utilizará
const IPAddress GATEWAY = IPAddress(192,168,3,1); // Gateway
const IPAddress SUBNET = IPAddress(255,255,255,0); // Máscara
// Google Public DNS represents two IP addresses for IPv4 – 8.8.8.8 and 8.8.4.4
const IPAddress PRIMARY_DNS(8, 8, 8, 8);   
const IPAddress SECONDARY_DNS(8, 8, 4, 4); 

const int port = 80; // Porta

//#define firebaseConfig = {
  //apiKey: "AIzaSyAgMMrDcBXjooAWGWyb16J3t4lDMOPg4ik",
  //authDomain: "automacao-residencial-83a57.firebaseapp.com",
  //databaseURL: "https://automacao-residencial-83a57.firebaseio.com",
 // projectId: "automacao-residencial-83a57",
  //storageBucket: "automacao-residencial-83a57.appspot.com",
  //messagingSenderId: "767821241457",
 // appId: "1:767821241457:web:6df13a75a668a15e48cfd0",
 // measurementId: "G-9LRXEY3EQ6"
//};

// Objeto WiFi Server, o ESP será o servidor
WiFiServer server(port); 

// Vetor com os clientes que se conectarão no ESP
std::vector<WiFiClient> clients; 

// Task que insere novos clientes (recém conectados) no vector
void taskNewClients(void *);
// Task que recebe executa comandos dos clientes
void handleClients(void *);

int LED = 2;
const int freq = 50; 
const int canal = 0; 
const int resolucao = 12; 

const int pinoAtuacao = 13; 
int ciclo = 200; 


// Conectamos no WiFi e iniciamos o servidor
void serverBegin()
{ 
  // Iniciamos o WiFi  
  WiFi.begin(ssid, password);  
  Serial.println("Connecting to WiFi");
  
  // Enquanto não estiver conectado exibimos um ponto
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  // Exibimos na serial
  Serial.println("OK");

  //Configuramos o WiFi com o IP definido anteriormente 
  if (!WiFi.config(IP, GATEWAY, SUBNET, PRIMARY_DNS, SECONDARY_DNS)) 
  {
    Serial.println("STA Failed to configure");
    while(1);
  }

  // Printamos o IP (debug)
  Serial.println(WiFi.localIP());
    
  // Iniciamos o servidor
  server.begin(port);
  delay(1000);
}

// Criamos as 2 Tasks
void createTasks()
{    
  //Criamos a task que insere os novos clientes no vector
  xTaskCreatePinnedToCore(taskNewClients, "taskNewClients", 10000, NULL, 2, NULL, 1);

  //Criamos a task que recebe e executa os comandos dos clients conectados
  xTaskCreatePinnedToCore(handleClients, "handleClients", 10000, NULL, 2, NULL, 1);  
}

void setup() 
{  
  // Iniciamos a serial com 115200 de velocidade
  Serial.begin(115200);  
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode(pinoAtuacao, OUTPUT); 
  ledcSetup(canal, freq, resolucao); 
  ledcAttachPin(pinoAtuacao, canal); 
  //Iniciamos o valor do servo com 150, que se equivale a 0.
  ledcWrite(canal, 150);   

  // Iniciamos o servidor (WiFi Server)
  serverBegin();
  // Criamos 3 tasks (mais detalhes no escopo da função)
  createTasks();  
  //Inicia a conexão com o Firebase0
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  //Muda o valor do Servo no firebase para 0.
  Firebase.setInt("Servo", 0);
  //Se está o comando é On, ligar o LED.
  //Callback quando tem alguma alteração no firebase
  Firebase.stream("/", [](FirebaseStream stream) {
    //Entra quando o evento que vem do callback é de alteração "put"
    if (stream.getEvent() == "put") 
    {
      
      //Verifica se o nome é o mesmo do Servo. O nome tem que ser igual com o nome no Firebase e no Aplicativo.
      if (stream.getPath() == "/Servo") {
        //Muda o valor do Servo de acordo com oque foi recebido e é feito o mapeamento do valor.
        ledcWrite(canal, map(stream.getDataInt(),0,180,150,450));   
      }
      else{
        String cmd;
        cmd=stream.getPath();
        cmd.toUpperCase();
        Serial.println("Recebido: ["+stream.getPath()+"]");
        if(cmd == "/LED"){
          cmd= stream.getDataString();
          cmd.toUpperCase();
          Serial.println("Recebido2: ["+cmd+"]");
          if(cmd=="ON"){
            
            digitalWrite(LED_BUILTIN, HIGH);
          }
          else{
            if(cmd=="OFF"){
              digitalWrite(LED_BUILTIN, LOW);
            }
          } 
        }
      }  
    }
  });
}


// Função que executa um comando de acordo com a String "cmd"
void executeCommand(String cmd, WiFiClient client)
{
  // String que guarda a resposta que será enviada para o client
  String response = "";

  // Se a String estiver vazia, abortamos a função
  if (cmd.equals("")) 
    return;

  // Exibimos o comando recebido na serial e no 
  Serial.println("Recebido: ["+cmd+"]");
  // Deixamos a string toda em maiúsculo
  cmd.toUpperCase();  
  //Verifica se o comando é destinado ao Servo
  if(cmd.indexOf("SERVO")==0)
  {
    //Recebe o grau de acordo com o valor após o espaço " "
    int grau = atoi((cmd.substring(cmd.indexOf(" ")+1).c_str()));

    //Mapeia o valor recebido para o valor do Servo
    ciclo = map(grau,0,180,150,450);
    ledcWrite(canal, ciclo); 
    
    response = "ok";
  }
  else{
    if(cmd.indexOf("LED ON")==0){
      digitalWrite(LED_BUILTIN, HIGH);
       response = "ok";
    }
    else{
      if(cmd.indexOf("LED OFF")==0){
        digitalWrite(LED_BUILTIN, LOW);
         response = "ok";
      }
      else{
        response = "Invalid Command";
      }
    }
    
  }
  // Enviamos para o client passado por parâmetro e exibimos sucesso ou falha na serial
  if(client.print(response)>0)
    Serial.println("Resposta enviada");
  else
    Serial.println("Erro ao enviar resposta"); 
}

// Task que insere novos clientes conectados no vector
void taskNewClients(void *p)
{
  // Objeto WiFiClient que receberá o novo cliente conectado
  WiFiClient newClient;
  // Tempo esperado no delay da task (1 ms)
  TickType_t taskDelay = 1 / portTICK_PERIOD_MS;

  while(true)
  {    
    // Se existir um novo client atribuimos para a variável
    newClient = server.available(); 
    
    // Se o client for diferente de nulo
    if(newClient)    
    {      
      // Inserimos no vector
      clients.push_back(newClient);
      // Exibimos na serial indicando novo client e a quantidade atual de clients
      Serial.println("New client! size:"+String(clients.size()));
    }    
    // Aguardamos 1ms
    vTaskDelay(taskDelay);
  }
}

// Função que verifica se um ou mais clients se desconectaram do server e, se sim, estes clients serão retirados do vector
void refreshConnections()
{
  // Flag que indica se pelo menos um client ser desconectou
  bool flag = false;
  // Objeto que receberá apenas os clients conectados
  std::vector<WiFiClient> newVector;

  // Percorremos o vector
  for(int i=0; i<clients.size(); i++)
  {
    // Verificamos se o client está desconectado
    if(!clients[i].connected())
    {
      // Exibimos na serial que um cliente se desconectou e a posição em que ele está no vector (debug)
      Serial.println("Client disconnected! ["+String(i)+"]");
      // Desconectamos o client
      clients[i].stop();      
      // Setamos a flag como true indicando que o vector foi alterado
      flag = true;          
    }
    else
      newVector.push_back(clients[i]); // Se o client está conectado, adicionamos no newVector
  }  
  // Se pelo menos um client se desconectou, atribuimos ao vector "clients" os clients de "newVector"
  if(flag)
    clients = newVector;
}


// Função que verifica se um cliente enviou um comando
void handleClients(void *p)
{
  // String que receberá o comando
  String cmd;
  // Tempo aguardado pela task (1 ms)
  TickType_t taskDelay = 1 / portTICK_PERIOD_MS;

  // Loop infinito
  while(true)
  {    
    // Atualizamos o vector deixando somente os clientes conectados
    refreshConnections();

    // Percorremos o vector
    for(int i=0; i<clients.size(); i++)
    {      
      // Se existir dados a serem lidos
      if(clients[i].available())
      {
        // Recebemos a String até o '\n'
        cmd = clients[i].readStringUntil('\n');
        // Executamos um comando, enviando por parametro a String cmd, e o client que nos enviou o comando
        executeCommand(cmd, clients[i]);        
      }          
      // Delay de 1 ms
      esp_task_wdt_reset();
    }
    // Delay de 1 ms
    vTaskDelay(taskDelay); 
  }
}


// Função que exibe a quantidade de clientes conectados no servidor do ESP
void showClients()
{
  Serial.println("Clients: "+String(clients.size()));
}

int j=0;
void loop() 
{
  // Exibimos o total de clientes conectados
  showClients();
  delay(5000);   
}
=======


#include <Arduino.h> //Lib Arduino
#include <WiFi.h> // Lib WiFi
#include <Wire.h>  // Necessário apenas para o Arduino 1.6.5 e posterior
#include <SPI.h> // Lib de comunicação SPI
#include <vector> // Lib com as funções de vetor (vector)
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h> //Lib Firebase
#define FIREBASE_HOST "https://automacao-residencial-83a57.firebaseio.com" //databaseURL fornecido pelo Firebase para conexão
#define FIREBASE_AUTH ""
#include "esp_task_wdt.h"
const char* ssid = "MOTOROLA-A7CE9"; // Coloque o nome da sua rede wifi aqui
const char* password = "3104e43ece65b0885fa9"; // Coloque a sua senha wifi aqui

const IPAddress IP = IPAddress(192,168,0,15); // IP fixo que o ESP utilizará
const IPAddress GATEWAY = IPAddress(192,168,3,1); // Gateway
const IPAddress SUBNET = IPAddress(255,255,255,0); // Máscara
// Google Public DNS represents two IP addresses for IPv4 – 8.8.8.8 and 8.8.4.4
const IPAddress PRIMARY_DNS(8, 8, 8, 8);   
const IPAddress SECONDARY_DNS(8, 8, 4, 4); 

const int port = 80; // Porta

//#define firebaseConfig = {
  //apiKey: "AIzaSyAgMMrDcBXjooAWGWyb16J3t4lDMOPg4ik",
  //authDomain: "automacao-residencial-83a57.firebaseapp.com",
  //databaseURL: "https://automacao-residencial-83a57.firebaseio.com",
 // projectId: "automacao-residencial-83a57",
  //storageBucket: "automacao-residencial-83a57.appspot.com",
  //messagingSenderId: "767821241457",
 // appId: "1:767821241457:web:6df13a75a668a15e48cfd0",
 // measurementId: "G-9LRXEY3EQ6"
//};

// Objeto WiFi Server, o ESP será o servidor
WiFiServer server(port); 

// Vetor com os clientes que se conectarão no ESP
std::vector<WiFiClient> clients; 

// Task que insere novos clientes (recém conectados) no vector
void taskNewClients(void *);
// Task que recebe executa comandos dos clientes
void handleClients(void *);

int LED = 2;
const int freq = 50; 
const int canal = 0; 
const int resolucao = 12; 

const int pinoAtuacao = 13; 
int ciclo = 200; 


// Conectamos no WiFi e iniciamos o servidor
void serverBegin()
{ 
  // Iniciamos o WiFi  
  WiFi.begin(ssid, password);  
  Serial.println("Connecting to WiFi");
  
  // Enquanto não estiver conectado exibimos um ponto
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  // Exibimos na serial
  Serial.println("OK");

  //Configuramos o WiFi com o IP definido anteriormente 
  if (!WiFi.config(IP, GATEWAY, SUBNET, PRIMARY_DNS, SECONDARY_DNS)) 
  {
    Serial.println("STA Failed to configure");
    while(1);
  }

  // Printamos o IP (debug)
  Serial.println(WiFi.localIP());
    
  // Iniciamos o servidor
  server.begin(port);
  delay(1000);
}

// Criamos as 2 Tasks
void createTasks()
{    
  //Criamos a task que insere os novos clientes no vector
  xTaskCreatePinnedToCore(taskNewClients, "taskNewClients", 10000, NULL, 2, NULL, 1);

  //Criamos a task que recebe e executa os comandos dos clients conectados
  xTaskCreatePinnedToCore(handleClients, "handleClients", 10000, NULL, 2, NULL, 1);  
}

void setup() 
{  
  // Iniciamos a serial com 115200 de velocidade
  Serial.begin(115200);  
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode(pinoAtuacao, OUTPUT); 
  ledcSetup(canal, freq, resolucao); 
  ledcAttachPin(pinoAtuacao, canal); 
  //Iniciamos o valor do servo com 150, que se equivale a 0.
  ledcWrite(canal, 150);   

  // Iniciamos o servidor (WiFi Server)
  serverBegin();
  // Criamos 3 tasks (mais detalhes no escopo da função)
  createTasks();  
  //Inicia a conexão com o Firebase0
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  //Muda o valor do Servo no firebase para 0.
  Firebase.setInt("Servo", 0);
  //Se está o comando é On, ligar o LED.
  //Callback quando tem alguma alteração no firebase
  Firebase.stream("/", [](FirebaseStream stream) {
    //Entra quando o evento que vem do callback é de alteração "put"
    if (stream.getEvent() == "put") 
    {
      
      //Verifica se o nome é o mesmo do Servo. O nome tem que ser igual com o nome no Firebase e no Aplicativo.
      if (stream.getPath() == "/Servo") {
        //Muda o valor do Servo de acordo com oque foi recebido e é feito o mapeamento do valor.
        ledcWrite(canal, map(stream.getDataInt(),0,180,150,450));   
      }
      else{
        String cmd;
        cmd=stream.getPath();
        cmd.toUpperCase();
        Serial.println("Recebido: ["+stream.getPath()+"]");
        if(cmd == "/LED"){
          cmd= stream.getDataString();
          cmd.toUpperCase();
          Serial.println("Recebido2: ["+cmd+"]");
          if(cmd=="ON"){
            
            digitalWrite(LED_BUILTIN, HIGH);
          }
          else{
            if(cmd=="OFF"){
              digitalWrite(LED_BUILTIN, LOW);
            }
          } 
        }
      }  
    }
  });
}


// Função que executa um comando de acordo com a String "cmd"
void executeCommand(String cmd, WiFiClient client)
{
  // String que guarda a resposta que será enviada para o client
  String response = "";

  // Se a String estiver vazia, abortamos a função
  if (cmd.equals("")) 
    return;

  // Exibimos o comando recebido na serial e no 
  Serial.println("Recebido: ["+cmd+"]");
  // Deixamos a string toda em maiúsculo
  cmd.toUpperCase();  
  //Verifica se o comando é destinado ao Servo
  if(cmd.indexOf("SERVO")==0)
  {
    //Recebe o grau de acordo com o valor após o espaço " "
    int grau = atoi((cmd.substring(cmd.indexOf(" ")+1).c_str()));

    //Mapeia o valor recebido para o valor do Servo
    ciclo = map(grau,0,180,150,450);
    ledcWrite(canal, ciclo); 
    
    response = "ok";
  }
  else{
    if(cmd.indexOf("LED ON")==0){
      digitalWrite(LED_BUILTIN, HIGH);
       response = "ok";
    }
    else{
      if(cmd.indexOf("LED OFF")==0){
        digitalWrite(LED_BUILTIN, LOW);
         response = "ok";
      }
      else{
        response = "Invalid Command";
      }
    }
    
  }
  // Enviamos para o client passado por parâmetro e exibimos sucesso ou falha na serial
  if(client.print(response)>0)
    Serial.println("Resposta enviada");
  else
    Serial.println("Erro ao enviar resposta"); 
}

// Task que insere novos clientes conectados no vector
void taskNewClients(void *p)
{
  // Objeto WiFiClient que receberá o novo cliente conectado
  WiFiClient newClient;
  // Tempo esperado no delay da task (1 ms)
  TickType_t taskDelay = 1 / portTICK_PERIOD_MS;

  while(true)
  {    
    // Se existir um novo client atribuimos para a variável
    newClient = server.available(); 
    
    // Se o client for diferente de nulo
    if(newClient)    
    {      
      // Inserimos no vector
      clients.push_back(newClient);
      // Exibimos na serial indicando novo client e a quantidade atual de clients
      Serial.println("New client! size:"+String(clients.size()));
    }    
    // Aguardamos 1ms
    vTaskDelay(taskDelay);
  }
}

// Função que verifica se um ou mais clients se desconectaram do server e, se sim, estes clients serão retirados do vector
void refreshConnections()
{
  // Flag que indica se pelo menos um client ser desconectou
  bool flag = false;
  // Objeto que receberá apenas os clients conectados
  std::vector<WiFiClient> newVector;

  // Percorremos o vector
  for(int i=0; i<clients.size(); i++)
  {
    // Verificamos se o client está desconectado
    if(!clients[i].connected())
    {
      // Exibimos na serial que um cliente se desconectou e a posição em que ele está no vector (debug)
      Serial.println("Client disconnected! ["+String(i)+"]");
      // Desconectamos o client
      clients[i].stop();      
      // Setamos a flag como true indicando que o vector foi alterado
      flag = true;          
    }
    else
      newVector.push_back(clients[i]); // Se o client está conectado, adicionamos no newVector
  }  
  // Se pelo menos um client se desconectou, atribuimos ao vector "clients" os clients de "newVector"
  if(flag)
    clients = newVector;
}


// Função que verifica se um cliente enviou um comando
void handleClients(void *p)
{
  // String que receberá o comando
  String cmd;
  // Tempo aguardado pela task (1 ms)
  TickType_t taskDelay = 1 / portTICK_PERIOD_MS;

  // Loop infinito
  while(true)
  {    
    // Atualizamos o vector deixando somente os clientes conectados
    refreshConnections();

    // Percorremos o vector
    for(int i=0; i<clients.size(); i++)
    {      
      // Se existir dados a serem lidos
      if(clients[i].available())
      {
        // Recebemos a String até o '\n'
        cmd = clients[i].readStringUntil('\n');
        // Executamos um comando, enviando por parametro a String cmd, e o client que nos enviou o comando
        executeCommand(cmd, clients[i]);        
      }          
      // Delay de 1 ms
      esp_task_wdt_reset();
    }
    // Delay de 1 ms
    vTaskDelay(taskDelay); 
  }
}


// Função que exibe a quantidade de clientes conectados no servidor do ESP
void showClients()
{
  Serial.println("Clients: "+String(clients.size()));
}

int j=0;
void loop() 
{
  // Exibimos o total de clientes conectados
  showClients();
  delay(5000);   
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
