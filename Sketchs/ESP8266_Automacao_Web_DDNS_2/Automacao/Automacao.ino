<<<<<<< HEAD
//Incluimos as libs necessárias para nos conectarmos à wifi
//criarmos o servidor e utilizarmos o sistema de arquivos

#include <WebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

//Mude para os dados da sua rede
#define SSID "MOTOROLA-A7CE9"
#define PASSWORD "3104e43ece65b0885fa9"
#define IP "192.168.0.20"
#define GATEWAY "192.168.0.1"
#define SUBNET "255.255.255.0"
const char* http_username = "Lobo Alfa";
const char* http_password = "@Lobo#Alfa@";
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

//Quantidade máxima de pinos, lembrando que os gpios geralmente começam em 0
//Se o gpio máximo for 16, por exemplo, coloque 17
#define MAX_PIN_COUNT 35
//Tamanho máximo para o nome que será vinculado ao pino
#define MAX_PIN_NAME 20

//Estrutura que guardará as informações dos pinos
typedef struct
{
    //Nome vinculado ao pino, para aparecer na lista de botões
    char name[MAX_PIN_NAME];
    //1 para ligado, 0 para desligado
    int value;
    //Se está em uso, ou seja, deve aparecer na lista
    bool isInUse;
}Pin;

//Servidor que vai rodar na porta 80, padrão http
WebServer server(8221);
//Lista de pinos
Pin pins[MAX_PIN_COUNT];

void setup()
{  
    //Serial apenas para debug
    Serial.begin(9600);
    //Incializa o sistema de arquivos
    SPIFFS.begin();
    //Envia o arquivo html quando entrar quando fizerem uma requisição na raiz do servidor
    server.serveStatic("/", SPIFFS, "/data/automacao.html");

    //Carrega as configurações salvas em arquivo
    loadConfig();

    //Conecta o ESP ao nosso roteador
    WiFi.begin(SSID, PASSWORD);
    
    Serial.println("");
    Serial.print("Conectando");

    //Espera enquanto não conecta ao roteador
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    //Configurações da rede
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    ip.fromString(IP);
    gateway.fromString(GATEWAY);
    subnet.fromString(SUBNET);

    Serial.println(ip);

    //Envia para o roteador as configurações que queremos para o ESP
    WiFi.config(ip, gateway, subnet);

    Serial.println(WiFi.localIP());

    Serial.println("");
    Serial.println("Conectado");

    //Rota para adicionar um novo pino à lista
    server.on("/addPin", HTTP_POST, onAddPin);
    //Rota para remover um pino da lista
    server.on("/removePin", HTTP_POST, onRemovePin);
    //Rota mudar o estado de um pino
    server.on("/digitalWrite", HTTP_POST, onDigitalWrite);
    //Rota para retornar a lista de pinos em uso, com nome e valor do estado atual
    server.on("/pinList", HTTP_GET, onPinList);
    //Quando fizerem requisição em uma rota não declarada
    server.onNotFound(onNotFound);
    //Inicializa o nosso servidor
    server.begin();
}

//Função executada quando fizerem uma requisição POST na rota '/addPin'
void onAddPin()
{
    //Se passaram os argumentos necessários no post
    if(server.hasArg("pinName") && server.hasArg("pinNumber"))
    {
        //Gpio do pino que quer adicionar
        int pinNumber = server.arg("pinNumber").toInt();

        //Se o pino não estiver em uso
        if(!pins[pinNumber].isInUse)
        {
            //Variável com o nome do botão vinculado ao pino
            char name[MAX_PIN_NAME];
            String argPinName = server.arg("pinName");
            //Copia o nome do argumento para a variável
            argPinName.toCharArray(name, MAX_PIN_NAME);
            strcpy(pins[pinNumber].name, name);
            //Marca o pino como em uso, para poder aparecer na lista
            pins[pinNumber].isInUse = true;
            //Salva em arquvio as configurações dos pinos
            saveConfig();
            //Retorna sucesso
            server.send(200, "text/plain", "OK");
        }
        //Pino já está em uso
        else
        {
            //Retorna aviso sobre pino em uso
            server.send(406, "text/plain", "Not Acceptable - Pin is not Available");
        }
    }
    //Parâmetros incorretos
    else
    {
        //Retorna aviso sobre parâmetros incorretos
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Função executada quando fizerem uma requisição POST na rota '/removePin'
void onRemovePin()
{
    //Se o parâmetro foi passado
    if(server.hasArg("pinNumber"))
    {
        //Verifica qual o gpio vai ficar livre
        int pinNumber = server.arg("pinNumber").toInt();
        //Marca o pino para dizer que não está mais em uso, fazendo com que ele não apareça mais na lista
        pins[pinNumber].isInUse = false;
        //Salva as configurações em arquivo
        saveConfig();
        //Retorna sucesso
        server.send(200, "text/plain", "OK");
    }
    else
    {
        //Retorna aviso sobre parâmetro faltando
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Função que será executada quando fizerem uma requsição do tipo GET na rota 'pinList'
void onPinList()
{
    //Cria um json que inicialmente mostra qual o valor máximo de gpio
    String json = "{\"count\":"+String(MAX_PIN_COUNT);
    //Lista de pinos
    json += ",\"pins\":[";
    for(int i=0; i < MAX_PIN_COUNT; i++)
    {
        //Se o pino está marcado como em uso
        if(pins[i].isInUse)
        {
            //Adiciona no json as informações sobre este pino
            json += "{";
            json += "\"name\":\"" + String(pins[i].name) + "\",";
            json += "\"number\":" + String(i) + ",";
            json += "\"value\":" + String(pins[i].value);
            json += "},";
        }
    }
    json += "]}";
    //Remove a última virgula que não é necessário após o último elemento
    json.replace(",]}", "]}");
    //Retorna sucesso e o json
    server.send(200, "text/json", json);
}

//Função que será executada quando fizerem uma requisição do tipo POST na rota 'digitalWrite'
void onDigitalWrite()
{
    //Se a requisição possui os parâmetros que precisamos
    if(server.hasArg("pinNumber") && server.hasArg("pinValue"))
    {
        //Número do pino
        int number = server.arg("pinNumber").toInt();
        //Valor para o qual ele deve ir
        int value = server.arg("pinValue").toInt();
        //Validação para que 'value' possua apenas 0 ou 1
        value = value <= 0 ? 0 : 1;
        //Coloca o pino como modo de saída
        pinMode(number, OUTPUT);
        //Muda o estado do pino para LOW caso o 'value' for 0, ou para HIGH caso 'value' for 1
        digitalWrite(number, value);
        //Salva o valor
        pins[number].value = value;
        //Salva em arquivo
        saveConfig();
        //Retorna sucesso
        server.send(200, "text/plain", "OK");
    }
    //Caso esteja faltando parâmetros
    else
    {
        //Envia aviso sobre parâmetros faltando
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Salva em arquivo as informações sobre os pinos
//para que seja possível recuperar estas informações
//mesmo depois que o ESP for rebootado
void saveConfig()
{
    //Abrimos o arquivo em modo de escrita
    File f = SPIFFS.open("/config.bin", "w");
    //Salvamos a lista com as informações sobre os pinos
    f.write((uint8_t *)pins, sizeof(Pin)*MAX_PIN_COUNT);
    //Fechamos o arquivo
    f.close();
}

//Recupera as informações sobre os pinos em uso
//para que seja possível recuperar o estado em
//que o ESP estava mesmo após rebootado
void loadConfig()
{
    File f;
    //Se o arquivo existe
    if((f = SPIFFS.open("/config.bin", "r")) != NULL)
    {
        //Lemos as informações dos pinos em colocamos na varável 'pins'
        f.read((uint8_t *)pins, sizeof(Pin)*MAX_PIN_COUNT);
        //Fechamos o arquivo
        f.close();
    }

    //Para cada pino
    for(int i=0; i<MAX_PIN_COUNT; i++)
    {
        //Se ele estiver em uso
        if(pins[i].isInUse)
        {
            //Coloca como saída
            pinMode(i, OUTPUT);
            //Altera o estado (HIGH ou LOW) de acordo com o que
            //estava salvo no arquivo
            digitalWrite(i, pins[i].value);
        }
    }
}

//Função que será executada quando fizerem uma requisição
//em uma rota que não foi declarada
void onNotFound() 
{
    //Retornamos aviso de não encontrado
    server.send(404, "text/plain", "Not Found");
}

//Loop principal
void loop()
{
    //Importante chamar o 'server.handleClient()'
    //para que o servidor verifique as requsições
    //e faça o roteamento para as funções correspondentes
    server.handleClient();
}
=======
//Incluimos as libs necessárias para nos conectarmos à wifi
//criarmos o servidor e utilizarmos o sistema de arquivos

#include <WebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

//Mude para os dados da sua rede
#define SSID "MOTOROLA-A7CE9"
#define PASSWORD "3104e43ece65b0885fa9"
#define IP "192.168.0.20"
#define GATEWAY "192.168.0.1"
#define SUBNET "255.255.255.0"
const char* http_username = "Lobo Alfa";
const char* http_password = "@Lobo#Alfa@";
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

//Quantidade máxima de pinos, lembrando que os gpios geralmente começam em 0
//Se o gpio máximo for 16, por exemplo, coloque 17
#define MAX_PIN_COUNT 35
//Tamanho máximo para o nome que será vinculado ao pino
#define MAX_PIN_NAME 20

//Estrutura que guardará as informações dos pinos
typedef struct
{
    //Nome vinculado ao pino, para aparecer na lista de botões
    char name[MAX_PIN_NAME];
    //1 para ligado, 0 para desligado
    int value;
    //Se está em uso, ou seja, deve aparecer na lista
    bool isInUse;
}Pin;

//Servidor que vai rodar na porta 80, padrão http
WebServer server(8221);
//Lista de pinos
Pin pins[MAX_PIN_COUNT];

void setup()
{  
    //Serial apenas para debug
    Serial.begin(9600);
    //Incializa o sistema de arquivos
    SPIFFS.begin();
    //Envia o arquivo html quando entrar quando fizerem uma requisição na raiz do servidor
    server.serveStatic("/", SPIFFS, "/data/automacao.html");

    //Carrega as configurações salvas em arquivo
    loadConfig();

    //Conecta o ESP ao nosso roteador
    WiFi.begin(SSID, PASSWORD);
    
    Serial.println("");
    Serial.print("Conectando");

    //Espera enquanto não conecta ao roteador
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    //Configurações da rede
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    ip.fromString(IP);
    gateway.fromString(GATEWAY);
    subnet.fromString(SUBNET);

    Serial.println(ip);

    //Envia para o roteador as configurações que queremos para o ESP
    WiFi.config(ip, gateway, subnet);

    Serial.println(WiFi.localIP());

    Serial.println("");
    Serial.println("Conectado");

    //Rota para adicionar um novo pino à lista
    server.on("/addPin", HTTP_POST, onAddPin);
    //Rota para remover um pino da lista
    server.on("/removePin", HTTP_POST, onRemovePin);
    //Rota mudar o estado de um pino
    server.on("/digitalWrite", HTTP_POST, onDigitalWrite);
    //Rota para retornar a lista de pinos em uso, com nome e valor do estado atual
    server.on("/pinList", HTTP_GET, onPinList);
    //Quando fizerem requisição em uma rota não declarada
    server.onNotFound(onNotFound);
    //Inicializa o nosso servidor
    server.begin();
}

//Função executada quando fizerem uma requisição POST na rota '/addPin'
void onAddPin()
{
    //Se passaram os argumentos necessários no post
    if(server.hasArg("pinName") && server.hasArg("pinNumber"))
    {
        //Gpio do pino que quer adicionar
        int pinNumber = server.arg("pinNumber").toInt();

        //Se o pino não estiver em uso
        if(!pins[pinNumber].isInUse)
        {
            //Variável com o nome do botão vinculado ao pino
            char name[MAX_PIN_NAME];
            String argPinName = server.arg("pinName");
            //Copia o nome do argumento para a variável
            argPinName.toCharArray(name, MAX_PIN_NAME);
            strcpy(pins[pinNumber].name, name);
            //Marca o pino como em uso, para poder aparecer na lista
            pins[pinNumber].isInUse = true;
            //Salva em arquvio as configurações dos pinos
            saveConfig();
            //Retorna sucesso
            server.send(200, "text/plain", "OK");
        }
        //Pino já está em uso
        else
        {
            //Retorna aviso sobre pino em uso
            server.send(406, "text/plain", "Not Acceptable - Pin is not Available");
        }
    }
    //Parâmetros incorretos
    else
    {
        //Retorna aviso sobre parâmetros incorretos
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Função executada quando fizerem uma requisição POST na rota '/removePin'
void onRemovePin()
{
    //Se o parâmetro foi passado
    if(server.hasArg("pinNumber"))
    {
        //Verifica qual o gpio vai ficar livre
        int pinNumber = server.arg("pinNumber").toInt();
        //Marca o pino para dizer que não está mais em uso, fazendo com que ele não apareça mais na lista
        pins[pinNumber].isInUse = false;
        //Salva as configurações em arquivo
        saveConfig();
        //Retorna sucesso
        server.send(200, "text/plain", "OK");
    }
    else
    {
        //Retorna aviso sobre parâmetro faltando
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Função que será executada quando fizerem uma requsição do tipo GET na rota 'pinList'
void onPinList()
{
    //Cria um json que inicialmente mostra qual o valor máximo de gpio
    String json = "{\"count\":"+String(MAX_PIN_COUNT);
    //Lista de pinos
    json += ",\"pins\":[";
    for(int i=0; i < MAX_PIN_COUNT; i++)
    {
        //Se o pino está marcado como em uso
        if(pins[i].isInUse)
        {
            //Adiciona no json as informações sobre este pino
            json += "{";
            json += "\"name\":\"" + String(pins[i].name) + "\",";
            json += "\"number\":" + String(i) + ",";
            json += "\"value\":" + String(pins[i].value);
            json += "},";
        }
    }
    json += "]}";
    //Remove a última virgula que não é necessário após o último elemento
    json.replace(",]}", "]}");
    //Retorna sucesso e o json
    server.send(200, "text/json", json);
}

//Função que será executada quando fizerem uma requisição do tipo POST na rota 'digitalWrite'
void onDigitalWrite()
{
    //Se a requisição possui os parâmetros que precisamos
    if(server.hasArg("pinNumber") && server.hasArg("pinValue"))
    {
        //Número do pino
        int number = server.arg("pinNumber").toInt();
        //Valor para o qual ele deve ir
        int value = server.arg("pinValue").toInt();
        //Validação para que 'value' possua apenas 0 ou 1
        value = value <= 0 ? 0 : 1;
        //Coloca o pino como modo de saída
        pinMode(number, OUTPUT);
        //Muda o estado do pino para LOW caso o 'value' for 0, ou para HIGH caso 'value' for 1
        digitalWrite(number, value);
        //Salva o valor
        pins[number].value = value;
        //Salva em arquivo
        saveConfig();
        //Retorna sucesso
        server.send(200, "text/plain", "OK");
    }
    //Caso esteja faltando parâmetros
    else
    {
        //Envia aviso sobre parâmetros faltando
        server.send(400, "text/plain", "Bad Request - Missing Parameters");
    }
}

//Salva em arquivo as informações sobre os pinos
//para que seja possível recuperar estas informações
//mesmo depois que o ESP for rebootado
void saveConfig()
{
    //Abrimos o arquivo em modo de escrita
    File f = SPIFFS.open("/config.bin", "w");
    //Salvamos a lista com as informações sobre os pinos
    f.write((uint8_t *)pins, sizeof(Pin)*MAX_PIN_COUNT);
    //Fechamos o arquivo
    f.close();
}

//Recupera as informações sobre os pinos em uso
//para que seja possível recuperar o estado em
//que o ESP estava mesmo após rebootado
void loadConfig()
{
    File f;
    //Se o arquivo existe
    if((f = SPIFFS.open("/config.bin", "r")) != NULL)
    {
        //Lemos as informações dos pinos em colocamos na varável 'pins'
        f.read((uint8_t *)pins, sizeof(Pin)*MAX_PIN_COUNT);
        //Fechamos o arquivo
        f.close();
    }

    //Para cada pino
    for(int i=0; i<MAX_PIN_COUNT; i++)
    {
        //Se ele estiver em uso
        if(pins[i].isInUse)
        {
            //Coloca como saída
            pinMode(i, OUTPUT);
            //Altera o estado (HIGH ou LOW) de acordo com o que
            //estava salvo no arquivo
            digitalWrite(i, pins[i].value);
        }
    }
}

//Função que será executada quando fizerem uma requisição
//em uma rota que não foi declarada
void onNotFound() 
{
    //Retornamos aviso de não encontrado
    server.send(404, "text/plain", "Not Found");
}

//Loop principal
void loop()
{
    //Importante chamar o 'server.handleClient()'
    //para que o servidor verifique as requsições
    //e faça o roteamento para as funções correspondentes
    server.handleClient();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
