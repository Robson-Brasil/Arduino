<<<<<<< HEAD
//Programa: Arduino e bluetooth HC-05 com Blynk
//Autor: Arduino e Cia
//Emula a serial nos pinos digitais 4 e 5
#include <SoftwareSerial.h>
SoftwareSerial SerialBT(6, 5); // RX, TX
//Configuracoes biblioteca Blynk
#define BLYNK_PRINT SerialBT
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleSerialBLE.h>
//Token de autorizacao
char auth[] = "PXcLbQrnASpT-556y0ganpQXOuhjNYoA";
void setup()
{
  //Inicializa a comunicacao serial com o modulo HC-05
  SerialBT.begin(38400);
  Blynk.begin(SerialBT, auth);
}
void loop()
{
  //Aguarda conexao e interpreta os comandos enviados
  Blynk.run();
}
=======
//Programa: Arduino e bluetooth HC-05 com Blynk
//Autor: Arduino e Cia
//Emula a serial nos pinos digitais 4 e 5
#include <SoftwareSerial.h>
SoftwareSerial SerialBT(6, 5); // RX, TX
//Configuracoes biblioteca Blynk
#define BLYNK_PRINT SerialBT
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleSerialBLE.h>
//Token de autorizacao
char auth[] = "PXcLbQrnASpT-556y0ganpQXOuhjNYoA";
void setup()
{
  //Inicializa a comunicacao serial com o modulo HC-05
  SerialBT.begin(38400);
  Blynk.begin(SerialBT, auth);
}
void loop()
{
  //Aguarda conexao e interpreta os comandos enviados
  Blynk.run();
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
