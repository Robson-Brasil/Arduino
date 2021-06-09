#include <TinyGPS++.h> // Bibliteca com as funções de GPS
#include <HardwareSerial.h> // Biblioteca Hardware Serial
#include "display/DisplayLoRa.h" // Biblioteca com as funções do display do ESP LoRa

// Objeto HardwareSerial. Comunicação com o módulo A9G
HardwareSerial A9G(1);
// Objeto com as funções de GPS
TinyGPSPlus gps;

// Pinos de comunicação Serial com o módulo A9G
const int RX_PIN = 23;
const int TX_PIN = 22;

// Pino do botão que configura o módulo caso pressionado durante o boot
const int btnConf = 37;

// Objeto do display
DisplayLoRa Display;

// Variáveis auxiliares de contegem de tempo
long prevMillis;
bool updateMillis = true;

// Assinatura de funções declaradas abaixo do loop
void displayInfo();
void applyTimeZone(int8_t delta, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *hours);
uint8_t getMonthLength(uint8_t month, uint16_t year);

// Exibimos na serial e no display a String 'msg'
void printMsg(String msg, bool clear)
{
  Serial.println(msg);
  Display.println(msg, clear);  
}

// Função que aguarda um tempo sem que seja necessário a utilização do delay
bool timeout(const int DELAY, long *millisAnterior, bool *flag)
{
  if(*flag)
  {
    *millisAnterior = millis();
    *flag = false;
  }  

  if((*millisAnterior + DELAY) < millis())
  {
    *flag = true;
    return true;
  }

  return false;
}

// Função que envia um comando para o módulo e aguarda um tempo de resposta
bool sendToA9G(String cmd, String *response, int timeoutWait = 3000)
{  
  *response = "";

  // Envia o comando pro módulo
  A9G.println(cmd);

  // Aguarda resposta do módulo
  updateMillis = true;
  while(!timeout(timeoutWait, &prevMillis, &updateMillis))
  {
    if(A9G.available())
    {
      *response = A9G.readString();
      return true;
    }
  }
  return false;
}

// Função que habilita e inicia o GPS
void A9config()
{
  String response = "";

  // Verifica se o modulo está funcionando
  while(response.indexOf("OK")<0)
  {
    printMsg("Enviando AT", true); 
    
    if(!sendToA9G("AT", &response))
      printMsg("Sem resposta...", true); 
    else
      printMsg("Resposta: "+response, true); 
  }
  printMsg("Modulo A9G OK", true); 

  // Habilita o GPS
  if(!sendToA9G("AT+GPS=1", &response))
  {
    printMsg("AT+GPS=1 erro", true); 
    while(true);
  }
  
  // Exibe OK ou erro
  if(response.indexOf("OK")<=0)
    printMsg("Erro ao receber ok, resposta: "+response, true); 
  else
    printMsg("AT+GPS=1 OK", true); 

  // Inicia o GPS, a partir desse momento o A9G nos enviará os dados de GPS de tempo e tempo
  if(!sendToA9G("AT+GPSRD=1", &response))
    printMsg("AT+GPSRD=1 erro", true); 

  // Exibe OK ou erro  
  if(response.indexOf("OK")<=0)
    printMsg("Erro ao receber ok, resposta: "+response, true); 
  else
    printMsg("AT+GPSRD=1 OK", true); 
}

void setup() 
{
  // Inicia comunicação Serial com o módulo. A velocidade padrão é 115200
  A9G.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  // Inicia a Serial que será usada como debug
  Serial.begin(115200);

  // Inicia o display
  Display.begin();  

  // Seta o pino do botão como entrada
  pinMode(btnConf, INPUT);

  // Exibe mensagem de teste na serial e no display
  printMsg("Test display", true); 

  // Aguarda 5 segundos, se o botão estiver pressionado nesse tempo, configuramos o módulo.
  updateMillis = true;
  while(!timeout(5000, &prevMillis, &updateMillis))
  {
    if(digitalRead(btnConf))
    {
      A9config();
      break;
    }
  }

  // Ignora as mensagens iniciais
  while(A9G.available() > 0)
    A9G.read();
}

void loop() 
{  
  // Se existirem dados a serem lidos, efetuamos a leitura e exibimos no display
  while(A9G.available() > 0)
    if(gps.encode(A9G.read()))
      displayInfo();
}

// Função que exibe os dados de GPS no display
void displayInfo()
{
  char location[30], date[30], time[30];  

  // Verificamos se os dados de GPS são válidos
  if(gps.location.isValid())
    // Se sim, atribuimos os valores de latitude e longitude para a variável location, separando-os por vírgula
    sprintf(location, "%.6f,%.6f", gps.location.lat(), gps.location.lng());        
  else
    // Se não, indicamos valor inválido
    strcpy(location, "INVALID"); 
  
  // Verificamos se os dados de data e tempo são válidos
  if(gps.date.isValid() && gps.time.isValid())
  { 
    uint8_t day, month, hours;
    uint16_t year;

    // Atribuímos os valores de dia, mes, ano e hora para as variáveis
    day = gps.date.day();
    month = gps.date.month();
    year = gps.date.year();
    hours = gps.time.hour();
    // Ajustamos o horário UTC-3 (Tempo Universal Coordenado)
    applyTimeZone(-3, &day, &month, &year, &hours);
    // Atribuímos os valores para a variável date
    sprintf(date, "%02d/%02d/%02d", day, month, year);
    // Atribuímos os valores para a variável time
    sprintf(time, "%02d:%02d:%02d", hours, gps.time.minute(), gps.time.second());
  }
  else
  {
    // Atribuímos uma mensagem de dados inválidos, se este for o caso
    strcpy(date, "INVALID");
    strcpy(time, "INVALID");
  }  

  // Exibimos na serial e no display os valores obtidos
  printMsg("Location: ", true);
  printMsg(String(location), false);
  printMsg("Date: "+ String(date), false);
  printMsg("Time: "+ String(time), false);
}

// Função que ajusta o horário UTC
void applyTimeZone(int8_t delta, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *hours) 
{
  if(delta < 0) 
  {
    if(int8_t(*hours) < -delta) 
    {
      *hours += 24+delta;
      if(--*day == 0) 
      {
        if(--*month == 0) 
        {
          *month = 12;
          *year--;
        }
        *day = getMonthLength(*month, *year);
      }
    } 
    else
      *hours += delta;
  } 
  else 
  {
    *hours += delta;
    if(*hours > 23) 
    {
      *hours -= 24;
      if(++*day > getMonthLength(*month, *year)) 
      {
        *day = 1;
        if(++*month > 12) 
        {
          *month = 1;
          *year++;
        }
      }
    }
  }
}

// Função auxiliar usada em "applyTimeZone", que obtém a qtde de dias no mês
uint8_t getMonthLength(uint8_t month, uint16_t year) 
{
  if(month == 2)
    if(year%4 == 0)
      return 29;
    else
      return 28;
  else 
  if((month == 4) || (month == 6) || (month == 9) || (month == 11))
    return 30;
  else
    return 31;
}

