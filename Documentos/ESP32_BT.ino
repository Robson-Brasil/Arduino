/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use ESP32 BT
  to connect your project to Blynk.

  Warning: Bluetooth support is in beta!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BT.h>

#define IN1 21
#define IN2 19
#define IN3 18
#define IN4 5
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YKeoyEFC2PUTpVnZOBEVxR7QRzKWSUsV";

void Frente();
void Re();
void Direita();
void Esquerda();
void Parar();

void setup()
{
  pinMode(IN1, OUTPUT);                  //Define LED vermelho como saída
  digitalWrite(IN1, LOW);                //LED vermelho deve iniciar apagado
  pinMode(IN2, OUTPUT);                 //Define LED verde como saída
  digitalWrite(IN2, LOW);               //LED verde deve iniciar apagado
  pinMode(IN3, OUTPUT);                 //Define LED amarelo como saída
  digitalWrite(IN3, LOW);               //LED amarelo deve iniciar apagado
  pinMode(IN4, OUTPUT);                 //Define LED azul como saída
  digitalWrite(IN4, LOW);               //LED azul deve iniciar apagado

  // Debug console
  Serial.begin(115200);

  Serial.println("Waiting for connections...");

  Blynk.setDeviceName("Carinho ESP32");

  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V1)                //Quando V1 do Blynk é acionado
{
  //Serial.println("Frente");
  Frente();
}

BLYNK_WRITE(V2)                //Quando V2 do Blynk é acionado
{
  Serial.println("Ré");
  Re;
}
BLYNK_WRITE(V3)                //Quando V3 do Blynk é acionado
{
  //Serial.println("Direita");
  Direita();
}

BLYNK_WRITE(V4)                //Quando V4 do Blynk é acionado
{
  //Serial.println("Esquerda");
  Esquerda();
}

BLYNK_WRITE(V5)                //Quando V4 do Blynk é acionado
{
  //Serial.println("Parar");
  Parar();
}

void Frente(){
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN4, HIGH); 
}

void Re(){
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); 
}
void Direita(){
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}
void Esquerda(){
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}
void Parar(){
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}
