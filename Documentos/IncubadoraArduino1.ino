/* Projeto de uma incubadora artificial de baixo custo utilizando a plataforma arduino 
 * Programa: incubadoraArduino1
 * Função: Monitorar e controlar a temperatura da incubadora, viragem dos ovos, e os dias do ciclo de incubação
 * Autor: Quéfrem Amorim da Silva
*/

// Diretivas de compilação

// include de blibiotecas locais
#include "DHT.h"

// include de blibiotecas padrões
#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>
#include <EEPROM.h>
#include<avr/wdt.h>


/*----------------------------Memória EEPROM----------------------------------*/
#define ESPACO_EEPROM 1000   //1Kb para todos os Arduinos com ATMega328P
// Fim Memória EEPROM

/*--------------------------Configuração do DHT22------------------------------*/
#define DHTPIN 2                        // Define a pinagem do Arduino onde estarÃ¡ ligado o sensor
#define DHTTYPE DHT22                   // Pode ser DHT11, 21, 22
DHT dht(DHTPIN, DHTTYPE);               // Cria o objeto dht
byte alimentacao_dht22 = 4;
float temperatura = 0;
byte umidade = 0;
// Variaveis para o controle de Umidade e Temperatura
byte umidade_atual = 0;                 // Variavel que armazena a temperatura lida do DHT22
float temperatura_atual = 0;            // Variavel que armazera a umidade lida do DHT22
// FIM definições DHT22

/*------------------------------Meu controle 2--------------------------------*/
unsigned int cont_tem_resis_des;        // Controle de tempo da resistencia Desligada
unsigned int cont_tem_resis_lig = 15;   // Controle de tempo da resistencia Ligada
float setpoint = 37.7;                  // Temperatura setada
float erro = 0;                         // Controle de erro da temperatura
float erro_ant = 0;                     // Controle de erro da temperatura
unsigned int segun;                     //##variavel para debugar o codigo
byte chama = 0;
//Fim meu controle

/*-------------Variaves Do rele para controle da resistencia e do motor de giragem-------------*/
byte resistencia = 8;                   // variavel da resistencia no IOU 13
byte motor = 7;                         // Variavel do motor de giragem no IOU 14
// Fim variaves do rele

/*-----------------------Variaveis para o controle do tempo de Incubação---------------------*/
byte dias_decorrido = 0;                // Variavel para armazenar os dias decorridos desde o começo da incubação
byte horas = 0;                         // Variavel para contar as horas desde o começo da incubação
byte minuto = 0;                        // Variavel para contar os minutos desde o começo da incubação
unsigned long controle_do_tempo;          // Variavel utilizada para tira a difernça ente o tempo decorrido
// Fim controle de Incubação

/*-----------------------Variaveis para configurações---------------------*/
int periodo_de_incubacao = 21;          // Variavel para definio o periodo de incubação
int temporizador = 2;                   // Variavel para definir o invervalo de tenmpo em que o motor de giragem dos ovos ira Ligar e Desligar
int liga_temporizador = 2;              // Variavel para definia apartir de que dia o temporizador vai ligar
int desliga_temporizador = 19;          // Variavel para definir apartir de que dia o temporizador vai Desligar
int erro_DHT22 = 0;

/*----------------------Configuração do display micro LCD--------------------*/
LCD_SSD1306 lcd; // Para módulo contralado pelo CI SSD1306 OLED */
// Fim micro LCD


//*******************************Função setup****************************************
void setup()
{
  //Serial.begin(9600);                           // Inicializa a serial em 9600 dps
  // Serial.print("teste Chocadaeira arduino");
   
  /*======================controle de temperaura e giragem dos ovos======================*/ 
  pinMode(resistencia, OUTPUT);           // Definindo pino da resistencia em modo de Saida de dados
  pinMode(motor, OUTPUT);                 // Definindo pino do motor em modo de saida de dados
  digitalWrite(resistencia, HIGH);        // Inicialdo resistencia Desligada
  digitalWrite(motor, HIGH);              // Iniciando motor de giragem Desligado

  // Controle das variaveis não volateis
  //Limpeza da EEPROM (Executado apenas para limpar a memoria. IMPORTANTE: Apos a primeira carga, comentar este trecho e efetuar a segunda carga em seguida.)
  dias_decorrido = EEPROM.read(0);        //Posicao 0 (zero) da EEPROM
  horas = EEPROM.read(1);                 //Posicao 0 (zero) da EEPROM 
  if (dias_decorrido > periodo_de_incubacao){
    for (int nL = 0; nL < ESPACO_EEPROM; nL++) { 
        EEPROM.write(nL, 0);
    }  //Comentar até aqui.
  }
  
  //Fim controle de temperaura e giragem dos ovos
  pinMode(alimentacao_dht22, OUTPUT);
  digitalWrite(alimentacao_dht22, HIGH);
  delay(250);
  dht.begin();                                  // Inicializa o sensor DHT22
  lcd.begin();                                  // Inicializa o Display Oled
  
  wdt_enable(WDTO_8S);
}


void AtualizaTela(){
  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_XLARGE);
  lcd.println("T " + String(temperatura)+" Cº");
  lcd.setFontSize(FONT_SIZE_XLARGE);
  lcd.println("U " + String(umidade)+" %");
  lcd.println("D:" + String(dias_decorrido) + " H:" + String(horas) + " M:" + String(minuto));
  lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.println("");
  lcd.println("Erro Leitura: "+ String(erro_DHT22));
  delay(250); 
}

void LeituraDHT22(){
  delay(3000);
  umidade_atual = dht.readHumidity();           // Faz a leitura da Umidade
  temperatura_atual = dht.readTemperature();    // Faz a Leitura da Temperatura
}


/*+++++++++++++++++++++++++++++++++++++++++++++Funação Controle de Temperatura++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void ControleTemperatura() {
  LeituraDHT22();
  while ( umidade_atual == 0 ){
    digitalWrite(alimentacao_dht22, LOW);
    delay(3000);
    digitalWrite(alimentacao_dht22, HIGH);
    delay(3000);
    umidade_atual = dht.readHumidity();           // Faz a leitura da Umidade
    temperatura_atual = dht.readTemperature();    // Faz a Leitura da Temperatura
    AtualizaTela();                               // Atualisando o Display
    erro_DHT22 = erro_DHT22 + 1;
  } 
  erro = setpoint - temperatura_atual;           // Calculo para verificar se har erro na temperatura


  /* Codigo onde verifico se a temperatura esta acima ou abaixo da temperatura setada
      seela estiver acima do ideal deve diminuir o tempo em que a resistencia fica ligada
      e se estiver abairo deve almenta o tempo em que a resistencia fica ligada e se por fim
      ela estiver correta não se deve mecher no tempo
  */
  if (erro == 0){
    
  } else if (erro-0.1 > 0 && erro != erro_ant) {            // Verivicado se a temperatura esta abaixo da ideal e se a difernça não é iguam a diferença anterior
    //Serial.println("erro"+String(erro));
    cont_tem_resis_lig += 2;                   // Tempo que a resistencia fica ligada deve ser somado com 500 mili segundo vezes o erro
    erro_ant = erro;                                  // Erro anterio deve recebe o erro atual
  } else if (erro+0.1 < 0 && erro != erro_ant) {      // Verificando se a  temperatura esta acima do ideal e se a diferença não é a diferença anterior
    cont_tem_resis_lig -= 1 ;                  // Tempo que a resistencia fica Desligado deve ser somado com 1000 mili segundo vezes o erro
    erro_ant = erro;                                  // Erro anterio deve recebe o erro atual
  }

  /* Verificano se ha ou não a necessidade de se ligar a resistencia se tiver necessidae devesse levar em concideração o erro
      da temperatura e o tempo que o sensor leva para perceber a diferença que a resistencia ligada fez na incubadora esse tempo
      é de mais ou menos 30 segundos durante esse tempo a resistencia deve-semanter deslidaga a contagem dos 30 segundos ocorre
      na função funPrincipal() para evitar que o Micro controlador fique travado durante esse tempo.
  */
  if (temperatura_atual < setpoint) {   // Verificado se a temperatura lida é menor a temperatura setada
    digitalWrite(resistencia, LOW );    // Liga resistencia
    //Serial.println("LIGADO   ");        //##mostra no serial monitor que a resistencia esta ligada para debug
    erro_ant = 0;                       // Erro Anterior recebe 0
    for(int i = 0; i < cont_tem_resis_lig; i++){
      wdt_reset();
      delay(500);
    }
   // delay(cont_tem_resis_lig);          // Deley para deixa a resistencia ligada durante determinado periodo de tempo
    digitalWrite(resistencia, HIGH);    // Desliga a resistencia
    //Serial.println("Desligado   ");     //##mostra a resistencia desligada para debug
    chama = 1;
    cont_tem_resis_des = millis();      // Começa a contar o tempo em que a resistencia esta desligada
   // Serial.println("cont1: "+String(cont_tem_resis_des));
   // delay(3000);
  } else {
    chama = 0;
  }
  //Serial.println("cont2: "+String(cont_tem_resis_des));
  //segun = (millis() - cont_tem_resis_des) / 1000;  //##mostra no monitor a quantos segundos a resitencia esta desligada para debug
  //Serial.println("T: " + String(temperatura_atual) + " U: " + String(umidade_atual) + "  contLIga: " + String(cont_tem_resis_lig) + "  segungos: " + String(segun));  //##mostra no monitos para debug

}
//Fim Função Controle de Temperatura Inicial____________________________________________________________________________________________


/*+++++++++++++++++++++++++++++++++++++++++++++++++++Função Girar Ovos+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void GiraOvos() {
  wdt_reset();
  if ((dias_decorrido >= liga_temporizador) && (dias_decorrido <= desliga_temporizador)) {   // Verifica se esta no tempo correto de gira os ovos
    if ((horas % temporizador) == 0) {     // Verificando se ja decorrerão as hora para ligar o temporizador
      digitalWrite(motor, LOW);            // Liga o motor de giragem
      for(int i = 0; i < 15; i++){         // Delay de 6 segundos
        wdt_reset();
        delay (500);
      }
      digitalWrite(motor, HIGH);           // Desliga o motor de giragem
    }
  }
}
//Fim Função Dira Ovos_____________________________________________________________________________________________________________________


//+++++++++++++++++++++++++++++++++++++++++++++++++Função de comtrole do tempo de incubação++++++++++++++++++++++++++++++++++++++++++++++++*/
void Relogio() {
  if ((millis() - controle_do_tempo) > 60000) { // Verivicado se ja se passaram 1 minuto
    minuto++ ;                                 // Minuto recebe minuto mais 1
    if (minuto > 59) {                        // Verificado se ja se passaram 59 Minutos
      horas++;                                // Horas recebe Horas mais 1
      if ( dias_decorrido <= periodo_de_incubacao){
        EEPROM.write(1, horas);        //Posicao 0 (zero) da EEPROM
      }
      minuto = 0;                             // Minutos recebe 0
      GiraOvos();                             // Chama a função girar ovos para verificar se ja esta na hora de ligar o temporizador
      if (horas >= 23) {                      // Verifica se ja se passaram 23 Horas
        dias_decorrido++;                     // Dias recebe dias mais 1
        if ( dias_decorrido <= periodo_de_incubacao){
          EEPROM.write(0, dias_decorrido);             //Posicao 1 (zero) da EEPROM
        }
        horas = 0;                            // horas recebe 0
      }
    }
    controle_do_tempo = millis();                 // Inicia a contagem do tempo novamente
  }
}
// FIM Função de controle de Incubação_____________________________________________________________________________________________________



//*******************************Função Loop****************************************
void loop()
{
    wdt_reset();
    LeituraDHT22();
    AtualizaTela();   // Chando a função Atualiza Tela
    int resul = millis() - cont_tem_resis_des;
  // Serial.print(millis());
  // Serial.println(" - " +String(cont_tem_resis_des));
  // Serial.println("Des: "+String(resul));
    if ( resul > 30000 || chama == 0) {  // Vericicando se ja passou o tempo que a resisitencia deve ficar desligada
      ControleTemperatura();                        // Chamando a função de controle de temperatura para ajusta as coisas
    }
    Relogio();        // Chando a função relogio    
}

