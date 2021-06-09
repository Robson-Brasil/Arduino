#include <Arduino.h> //biblioteca arduino (opcional)
#include <Adafruit_GFX.h>    //biblioteca do display grafico
#include <Fonts/FreeSans9pt7b.h> //fonte usada no display
#include <Adafruit_ST7735.h> // biblioteca de hardware do display
#include <SPI.h>  // biblioteca de comunicação SPI

#define TINY_GSM_MODEM_SIM800 // definição do modem usado (SIM800L)
#include <TinyGsmClient.h> // biblioteca com comandos GSM

// pinos display
#define TFT_CS 22 // CS
#define TFT_RST 21  // RESET
#define TFT_DC 5 // A0
#define TFT_MOSI 23 // SDA
#define TFT_CLK 18 // SCK

// objeto do display
Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

// tamanho da fonte do display
int fontHeight = 12;

// objeto de comunicação serial do SIM800L
HardwareSerial SerialGSM(1);

// objeto da bibliteca com as funções GSM
TinyGsm modemGSM(SerialGSM);

// velocidade da serial tanto do SIM800L quanto do monitor serial
const int BAUD_RATE = 9600;

// variáveis usadas para contar o tempo sem travar a função loop
// millis de referencia
long int millisRefCon, millisUserResp;
// flag que indica a contagem de tempo (usadas pela função 'timeout')
bool flagCon = false, flagUserResp = false;

// pinos aonde os reles serão ligados e RX / TX aonde o SIM800L será ligado
const int relayPin1 = 17, relayPin2 = 15, sensorPin = 16, RX_PIN = 4, TX_PIN = 2;

//Access point name da vivo
const char *APN = "zap.vivo.com.br";
//Usuario, se não existir deixe em vazio
const char *USER = "";
//Password, se não existir deixe em vazio
const char *PASSWORD = "";

// as variáveis abaixo usadas pela função loop
// flag que indica se, após a ligação feita pelo SIM800L, um usuario respondeu com um SMS em até 1min
bool userResponseSMS = false;
// flag que indica se o sensor está ativo
bool sensorActivated = false;
// index do vetor de numeros de celular, usado para percorrer o vetor
int i = 0;

// quantidade de celulares que receberão mensagens e ligações e poderão enviar comandos SMS
const int numbersTL = 2;
// numero de celulares, a ordem de chamada pelo programa é da esquerda para a direita
const String numbers[numbersTL] = {"+5518999999999", "+5518999999999"};

//Envia comando AT e aguarda até que uma resposta seja obtida
String sendAT(String command)
{
  String response = "";
  SerialGSM.println(command);
  // aguardamos até que haja resposta do SIM800L
  while(!SerialGSM.available());

  response = SerialGSM.readString();

  return response;
}

// inicializa GSM
void setupGSM()
{
  display.println("Setup GSM...");
  display.setTextColor(ST7735_GREEN);
  
  // inicia serial SIM800L
  SerialGSM.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN, false);
  delay(3000);

  // exibe info do modem no monitor serial
  Serial.println(modemGSM.getModemInfo());

  // inicia o modem
  if (!modemGSM.restart())
  {
    display.setTextColor(ST7735_RED);
    display.println("Restarting GSM\nModem failed");
    delay(10000);
    ESP.restart();
    return;
  }
  display.println("Modem restart OK");
  
  // aguarda network
  if (!modemGSM.waitForNetwork()) 
  {
    display.setTextColor(ST7735_RED);
    display.println("Failed to connect\nto network");
    delay(10000);
    ESP.restart();
    return;
  }
  display.println("Modem network OK");

  // conecta na rede (tecnologia GPRS)
  if(!modemGSM.gprsConnect(APN,USER,PASSWORD))
  {
    display.setTextColor(ST7735_RED);
    display.println("GPRS Connection\nFailed");
    delay(10000);
    ESP.restart();
    return;
  }
  
  display.println("GPRS Connect OK");

  //Define modo SMS para texto (0 = PDU mode, 1 = Text mode)
  if(sendAT("AT+CMGF=1").indexOf("OK") < 0)
  {
    display.setTextColor(ST7735_RED);
    display.println("SMS Txt mode Error");
    delay(10000);
    ESP.restart();
    return;
  }  
  display.println("SMS Txt mode OK");

  //Exclui todos SMS armazenados
  sendAT("AT + CMGD=1,4"); 
  resetDisplay();
  display.setTextColor(ST7735_WHITE);
}

// limpa e configura display
void resetDisplay()
{
  display.setRotation(1);
  display.setFont(&FreeSans9pt7b);
  display.fillScreen(ST77XX_BLACK);
  display.setTextColor(ST7735_WHITE);
  display.setCursor(0,fontHeight);
}

// executa chamada
void call(String number)
{
  display.print("Calling...");
  Serial.println(number);
  
  // tenta executar chamada
  bool res = modemGSM.callNumber(number);

  // se obteve sucesso exibe OK, se não, fail
  if(res)
    display.println(" OK");
  else
    display.println(" fail");

  if (res)
  {
    // assim que a chamada for feita é finalizada
    res = modemGSM.callHangup();

    // exibe se foi possível finalizar ou não
    display.print("Hang up: ");
    if(res)
      display.println("OK");
    else
      display.println("fail");
  }
}

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.println("Starting...");

  // seta pinos do sensor como entrada
  pinMode(sensorPin, INPUT);
  // seta pinos dos reles como saída
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // os reles trabalham com lógica inversa, setamos como HIGH para desligá-los de início
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  
  // atribui para as variáveis de contagem de tempo o tempo atual antes de entrar no loop
  millisRefCon = millisUserResp = millis();

  display.initR(INITR_BLACKTAB);
  resetDisplay();

  // inicia e configura o SIM800L
  setupGSM();
  resetDisplay();
  display.println("GPRS: Connected");
}

// Função que compara se o tempo foi atingido, sem que 'congele' a execução do loop
bool timeout(const int DELAY, long *previousMillis, bool *flag)
{
  if(*flag)
  {
    *previousMillis = millis();
    *flag = false;
  }

  if((*previousMillis + DELAY) < millis())
  {
    *flag = true;
    return true;
  }

  return false;
}

// envia SMS via GSM
bool sendSMS(String msg, String cel)
{
    Serial.println("Sending sms '" + msg + "'");
    
    if(modemGSM.sendSMS(cel, msg))
      return true;

    return false;
}

// obtem o numero que nos enviou o SMS
bool getSMSNumber(String cmglMsg, String *number)
{
  String msg;
  int i;

  *number = "";
  if(cmglMsg == "")
    return false;
    
  //Se a mensagem possui CMGL, ou seja, se ela é realmente uma mensagem SMS
  if(cmglMsg.indexOf("CMGL") >= 0)
  {    
    // obtém substring após a primeira virgula
    msg = cmglMsg.substring(cmglMsg.indexOf(",")+1);
    // obtém substring após a segunda virgula, pulando as aspas
    msg = msg.substring(msg.indexOf(",")+2);

    // percorre substring obtendo o numero do celular
    for(int i=0; i<msg.length() && msg.charAt(i)!='"'; i++)
      *number+=msg.charAt(i);

    return true;
  }

  return false;
}

// envia um SMS de resposta para o numero de celular que nos enviou um comando
void sendResponse(String number)
{
  String response = "Relay 1: ";

  // concatena string com o sinal atual dos reles
  // lógica inversa, HIGH -> desativado e LOW -> ativado
  if(digitalRead(relayPin1) == HIGH)
    response += "OFF\n";
  else
    response += "ON\n";

  response += "Relay 2: ";

  if(digitalRead(relayPin2) == HIGH)
    response += "OFF";
  else
    response += "ON";
  
  Serial.println("Trying to send sms");
  
  // envia SMS e exibe o sucesso / falha
  if(!sendSMS(response, number))
  {
    display.println("Fail to send SMS");
    Serial.println("Fail to send SMS");
  }
  else
  {
    display.println("SMS sent");
    Serial.println("SMS sent");
  }

  // exclui todos SMS armazenados
  sendAT("AT + CMGD=1,4");
}

// executa comando de acordo com a variavel 'msg'
bool commandOK(String number, String msg)
{
  // flag que indica se entrou em algum if (indicando comando válido)
  bool smsOK = false;

  // verifica se a msg corresponde a algum dos comandos existentes e seta os pinos correspondentes
  // os reles possuem lógica inversa
  if(msg.equalsIgnoreCase("relay 1 on"))
  {
    digitalWrite(relayPin1, LOW); 
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("relay 1 off")) 
  {
    digitalWrite(relayPin1, HIGH);
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("relay 2 on"))
  {
    digitalWrite(relayPin2, LOW); 
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("relay 2 off")) 
  {
    digitalWrite(relayPin2, HIGH);
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("relays off"))
  {
    digitalWrite(relayPin1, HIGH);
    digitalWrite(relayPin2, HIGH);
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("relays on"))
  {
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("ok")) // comando OK, usado para sinalizar alarme falso
    smsOK = true;
  else
  if(msg.equalsIgnoreCase("hello")) // comando hello, usado para verificar o funcionamento
  {
    modemGSM.sendSMS(number, "Hello!");
    smsOK = true;
  }
  else
  if(msg.equalsIgnoreCase("status")) // comando status que obtém os estados dos pinos
    smsOK = true;
  
  return smsOK;
}

// verifica se o número é algum dos declarados pelo programa
bool numberIsValid(String number)
{
  // percorre vetor com os números
  for(int i = 0; i < numbersTL; i++)
    if(numbers[i].equals(number))
      return true;

  return false;
}

// verifica se um sms é recebido e obtem o número de quem o enviou
bool SMSMessageRecv(String *msg, String *number)
{  
  // comando AT que lista todos os SMS armazenados
  *msg = sendAT("AT+CMGL=\"ALL\"");

  // se o SIM800L responder com SM, significa que um novo SMS acaba de chegar
  // então pedimos novamente que o SIM nos liste os SMS armazenados
  if((*msg).indexOf("+CMTI: \"SM\"")>=0)
      *msg = sendAT("AT+CMGL=\"ALL\"");

  // se a mensagem possui um OK e possui mais que 10 caracteres (existe pelo menos um SMS)
  if((*msg).indexOf("OK")>=0 && (*msg).length()>10)
  {
    // exibe a mensagem na serial
    Serial.println(*msg);

    // obtém o número que nos enviou o SMS e exibe se obteve sucesso
    if(getSMSNumber(*msg, *&number))
    {
      Serial.println("numero obtido: "+*number);
      return true;
    }
    else
    { 
      Serial.println("Erro ao obter numero");
      return false;
    }
  }
  // exibe na serial um ponto (debug)
  Serial.print(".");
 
  return false;
}

// obtem texto da mensagem e o retorna por parâmetro
void getTextSMS(String *msg)
{
  String aux;
/*
Exemplo de mensagmem:
[
+CMGL: 1,"REC UNREAD","+5518999999999","","18/11/30,11:36:14-08"
Hello

OK
]
*/
  //pula primeiro \n
  *msg = (*msg).substring((*msg).indexOf( "\n" )+1);
  //pula primeira linha: 'Cmd: +CMGL: 1,"REC UNREAD","+5518999999999","","18/11/30,11:04:30-08"'
  *msg = (*msg).substring((*msg).indexOf( "\n" )+1);

  aux = *msg;

  if(aux.length() <= 8)
    return;    

  *msg = "";
  // retira a substring "\r\n\r\nOK\r\n" (8 caracteres)
  for(int i=0; i<aux.length()-8; i++)
    *msg += aux.charAt(i);  
}

// verifica se o SIM800L se desconectou, se sim tenta reconectar
void verifyGPRSConnection()
{
  resetDisplay();
  display.print("GPRS: ");

  if(modemGSM.isGprsConnected())
    display.println("Connected");
  else
  {
    display.println("Disconnect");
    display.println("Reconnecting...");
    
    if(!modemGSM.waitForNetwork())
    {
      display.setTextColor(ST7735_RED);
      display.println("GPRS Con. Failed");

      delay(5000);
      display.setTextColor(ST7735_WHITE);
    }
    else
    {
      if(!modemGSM.gprsConnect(APN,USER,PASSWORD))
      {
        display.setTextColor(ST7735_RED);
        display.println("GPRS Con. Failed");
        delay(5000);
        display.setTextColor(ST7735_WHITE);
      }
      else
      {
        display.setTextColor(ST7735_GREEN);
        display.println("GPRS Con. OK");
      }
    }
  }
}

void executeCommand(String number, String msg)
{
  // se o número não é válido, exibe mensagem e não faz nada
  if(!numberIsValid(number))
  {
    display.println("Number is not valid");
    Serial.println("Number is not valid");
    Serial.println(number);
    delay(2500);
  }
  else
  {
    // se o número é valido
    // obtem o texto do SMS recebido
    Serial.println("Msg: '"+msg+"'");    
    getTextSMS(&msg);
    Serial.println("Cmd: '"+msg+"'");    

    // executa comando de acordo com o texto recebido
    if(commandOK(number, msg))
    {
      // sinaliza que o usuario respondeu com um SMS válido
      userResponseSMS = true;
      // retorna para false a flag que indica se sensor está ativo
      sensorActivated = false;
      
      display.println("Sending status");
      Serial.println("Sending status");

      // Envia o estado dos dois relés em um SMS
      sendResponse(number); 
    }
    else
    {
      // se o comando é inválido, exibe no display
      // sinaliza que o usuario respondeu com um SMS inválido
      userResponseSMS = false;

      display.println("Cmd is not valid");
      Serial.println("Cmd is not valid");
    }
  }
  // exclui todos os SMS armazenados
  sendAT("AT + CMGD=1,4"); 
}

bool isItToCall()
{
  // se o sensor de barreira está ativo ou foi uma vez ativado e não foi recebido um SMS em 1 min, retornamos true indicando que deve-se ligar parao próx número
  return digitalRead(sensorPin) == HIGH || (sensorActivated && !userResponseSMS && timeout(60000, &millisUserResp, &flagUserResp));
}

void loop()
{
  String msg, number;
  
  // de 5 em 5 segundos, verifica se o SIM800L está desconectado, se sim, tenta reconectar
  if(timeout(5000, &millisRefCon, &flagCon))
    verifyGPRSConnection();

  // se o SIM800L está conectado
  if(modemGSM.isGprsConnected())
  {
    // função que verifica se deve-se efetuar a chamada ou não
    if(isItToCall())
    {
      // sinaliza que o sensor foi ativado
      sensorActivated = true;
      userResponseSMS = false;
      // atribui à variavel de referencia para contar o tempo, o valor atual do millis
      millisUserResp = millis();

      Serial.println("Sensor activated!");
      Serial.println("Calling to number "+String(i+1));
      display.println("Sensor activated!");
      display.println("Calling to number "+String(i+1));

      // efetua a ligação para um dos nºs do vetor, iniciando com 0
      call(numbers[i++]);
      // após a chamada soma-se 1 ao i

      // se chegou ao fim do vetor, retorna ao início (0)
      if(i>=numbersTL)
        i = 0;
    }

    // verifica se foi recebido um SMS
    if(SMSMessageRecv(&msg, &number))
    {      
      // exibe mensagem no display e monitor serial
      resetDisplay();
      display.println("SMS Msg Received");
      Serial.println("SMS Msg Received");
      delay(2500);

      // validamos o SMS e executamos uma ação
      executeCommand(number, msg);
    }
  }
  else // exibe na serial que o modem está desconectado
    Serial.println("Disconnected");

  // único delay no loop de 10ms (desconsiderando a função de reconexão, que possui delay para exibição do display)
  delay(10);
}
























