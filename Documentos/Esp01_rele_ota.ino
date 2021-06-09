#include <ESP8266WiFi.h> //lib do wifi para o ESP8266
#include <ESP8266WiFiMulti.h> //lib do wifi para o ESP8266
#include <ArduinoOTA.h> //lib do ArduinoOTA

  //Cria um server na porta 80 (porta padrÃ£o para onde os navegadores enviam as requisiÃ§Ãµes http)
WiFiServer server(80);

ESP8266WiFiMulti wifiMulti;

const char* ssid = "*****"; //nome da rede
const char* password = "*****"; //senha da rede

void setup()
{
   Serial.begin(115200);


     //Configura o GPIO0 como output, ou seja, como saÃ­da para podermos alterar o seu valor
  pinMode(0, OUTPUT);

   Serial.println("Booting");   
   
   wifiMulti.addAP(ssid, password); 
   
   Serial.println("Connecting ...");
   
   while (wifiMulti.run() != WL_CONNECTED)
   {       
       delay(250);
       Serial.print('.');
   }

    //ConfiguraÃ§Ãµes do IP fixo. VocÃª pode alterar conforme a sua rede
  IPAddress ip(192, 168, 0, 111);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurando IP fixo para : ");
  Serial.println(ip);

  //Envia a configuraÃ§Ã£o
  WiFi.config(ip, gateway, subnet);

    //Inicializa o server que criamos na porta 80
  server.begin();

  //Mostramos no monitor serial o IP que o ESP possui para verificarmos se Ã© o mesmo que configuramos
  Serial.print("Server em: ");
  Serial.println(WiFi.localIP());

  // A porta fica default como 8266
  // ArduinoOTA.setPort(8266);

  // Define o hostname (opcional)
  ArduinoOTA.setHostname("esprele");

  // Define a senha (opcional)
  ArduinoOTA.setPassword("esp01releota");

  // É possível definir uma criptografia hash md5 para a senha usando a função "setPasswordHash"
  // Exemplo de MD5 para senha "admin" = 21232f297a57a5a743894a0e4a801fc3 
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  //define o que será executado quando o ArduinoOTA iniciar
  ArduinoOTA.onStart([](){ startOTA(); }); //startOTA é uma função criada para simplificar o código 

  //define o que será executado quando o ArduinoOTA terminar
  ArduinoOTA.onEnd([](){ endOTA(); }); //endOTA é uma função criada para simplificar o código 

  //define o que será executado quando o ArduinoOTA estiver gravando
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { progressOTA(progress,total); }); //progressOTA é uma função criada para simplificar o código 

  //define o que será executado quando o ArduinoOTA encontrar um erro
  ArduinoOTA.onError([](ota_error_t error){ errorOTA(error); });//errorOTA é uma função criada para simplificar o código 
  
  //inicializa ArduinoOTA
  ArduinoOTA.begin();
  
}

//#######################################################################################

//funções de exibição dos estágios de upload (start, progress, end e error) do ArduinoOTA
void startOTA()
{
   String type;
   
   //caso a atualização esteja sendo gravada na memória flash externa, então informa "flash"
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "flash";
    else  //caso a atualização seja feita pela memória interna (file system), então informa "filesystem"
      type = "filesystem"; // U_SPIFFS

    //exibe mensagem junto ao tipo de gravação
    Serial.println("Start updating " + type);
}

//exibe mensagem
void endOTA()
{
  Serial.println("\nEnd");
}

//exibe progresso em porcentagem
void progressOTA(unsigned int progress, unsigned int total)
{
   Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

//caso aconteça algum erro, exibe especificamente o tipo do erro
void errorOTA(ota_error_t error)
{  
      Serial.printf("Error[%u]: ", error);
      
      if (error == OTA_AUTH_ERROR) 
        Serial.println("Auth Failed");
      else
      if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else 
      if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else
      if (error == OTA_RECEIVE_ERROR) 
        Serial.println("Receive Failed");
      else 
      if (error == OTA_END_ERROR)
        Serial.println("End Failed");
}

//#######################################################################################

void loop() 
{
  //Handle é descritor que referencia variáveis no bloco de memória
  //Ele é usado como um "guia" para que o ESP possa se comunicar com o computador pela rede
  ArduinoOTA.handle();

  //código de atualização
  //Verifica se algum cliente estÃ¡ tentando se conectar
  WiFiClient client = server.available();
  if (!client)
  {
    //Se nÃ£o houver nenhum cliente podemos retornar pois nÃ£o hÃ¡ nada a fazer
    return;
  }

  Serial.println("Novo cliente conectou");

  //Fazemos a leitura da requisiÃ§Ã£o
  String req = client.readStringUntil('\r');
  Serial.print("RequisiÃ§Ã£o: ");
  Serial.println(req);

  //Este Ã© o html que iremos retornar para o cliente
  //Ã‰ composto basicamente de dois botÃµes (ON e OFF) para o GPIO0 e dois botÃµes (ON e OFF) para o GPIO2
  //A parte que nos interessa Ã© o <a href=' com a aÃ§Ã£o vinculada a cada botÃ£o
  //Quando clicamos em um destes botÃµes essa informaÃ§Ã£o chegarÃ¡ atÃ© o ESP para que ele verifique qual aÃ§Ã£o deve executar
  //A parte dentro de '<style>' Ã© apenas para modificarmos o visual da pÃ¡gina que serÃ¡ exibida, vocÃª pode alterÃ¡-la como queira
  String html = 
  "<html>"
    "<head>"
      "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
      "<title>ESP8266</title>"
      "<style>"
        "body{"
          "text-align: center;"
          "font-family: sans-serif;"
          "font-size:14px;"
          "padding: 25px;"
        "}"

        "p{"
          "color:#444;"
        "}"

        "button{"
          "outline: none;"
          "border: 2px solid #1fa3ec;"
          "border-radius:18px;"
          "background-color:#FFF;"
          "color: #1fa3ec;"
          "padding: 10px 50px;"
        "}"

        "button:active{"
          "color: #fff;"
          "background-color:#1fa3ec;"
        "}"
      "</style>"
    "</head>"
    "<body>"
    "<p>GPIO0</p>"
    "<p><a href='?acao=gpio0On'><button>ON</button></a></p>"
    "<p><a href='?acao=gpio0Off'><button>OFF</button></a></p>"
    "</body>"
  "</html>";

  //Escreve o html no buffer que serÃ¡ enviado para o cliente
  client.print(html);
  //Envia os dados do buffer para o cliente
  client.flush();

  //Verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio0On
  if (req.indexOf("acao=gpio0On") != -1)
  {
      //Se possui a aÃ§Ã£o gpio0On colocamos a saÃ­da do GPIO0 como alta
      digitalWrite(0, HIGH);
  }
  //SenÃ£o, verifica se a requisiÃ§Ã£o possui a aÃ§Ã£o gpio0Off
  else if (req.indexOf("acao=gpio0Off") != -1)
  {
    //Se possui a aÃ§Ã£o gpio0Off colocamos a saÃ­da do GPIO0 como baixa
    digitalWrite(0, LOW);
  }

}
