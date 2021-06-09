/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example to change UID of changeable MIFARE card.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to set the UID on a UID changeable MIFARE card.
 * NOTE: for more informations read the README.rst
 * 
 * @author Tom Clement
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SOM 7
#define RST_PIN   2     // Configurable, see typical pin layout above
#define SS_PIN    5    // Configurable, see typical pin layout above

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
const int relepin = 8;
int vezes = 3;
int leit;
int cartao = 0;
int cartaoplaca = 0;
int cont = 0;
unsigned long int code = 0;
unsigned long int code2 = 0;
uint8_t successRead;
byte readCard[4];
String conc;
String temp; 
String ope;
String pla;
String res;
String com;   // 40D4F179 D086C180
int pin = 2; //pino de interrupção (módulo IR) 
volatile unsigned int rpm; //contador de rpm 
  
void setup() {
  pinMode(SOM, OUTPUT);
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  pinMode(relepin, OUTPUT);
  lcd.init();
  lcd.backlight();
  com = "40D4F179D086C180";
attachInterrupt(digitalPinToInterrupt(pin), interruptPin, RISING); 
  rpm = 0;
}

void loop() {
 rpm = 0;
//////////// captura do codigo do cartão do operador /////////
operador();
Serial.print(ope);
//Serial.println(",");
delay(500);
////// função de captura do banco de dados /////
/// passo pendente

placa();
Serial.print(pla);
Serial.println(",");
delay(500);

//////// concatena string operador e veiculo /////////
res = ope += pla;
delay(500);

///////// verificação de permissão do conjunto operador + veiculo ////////
 if (res == com) {

  lcd.clear();  
  lcd.setCursor(3,0);
  lcd.print("INICIANDO");
  lcd.setCursor(1,1);
  lcd.print("ABASTECIMENTO");

  for(int j=0; j<vezes; j++)
  {
//  tone(SOM,700);
  digitalWrite(SOM, HIGH);
  delay(100);
 // noTone(SOM);
  delay(200);
  }
  digitalWrite(relepin, HIGH);
  delay(15000);
  digitalWrite(relepin, LOW);
  lcd.clear();  
  lcd.setCursor(2,0);
  lcd.print(rpm);
  lcd.setCursor(4,1);
  lcd.print("pulsos");
  delay(2000);
 }
 
 else{
 lcd.clear();  
 lcd.setCursor(3,0);
 lcd.print("PARAMENTROS");
 lcd.setCursor(3,1);
 lcd.print("INCORRETOS");
 delay(2000);
 lcd.clear();  
 lcd.setCursor(3,0);
 lcd.print("REINICIANDO");
 lcd.setCursor(4,1);
 lcd.print("SISTEMA");
 for(int j=0; j<vezes; j++)
  {
//  tone(SOM,1100);
  digitalWrite(SOM, HIGH);
  delay(500);
 // noTone(SOM);
  delay(100);
  }
 delay(1000);
}

}
void operador(){
  cartao = 0;
  code = 0;
  // Print a message to the LCD.
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("PASSE O CARTAO");
  lcd.setCursor(2,1);
  lcd.print("DO OPERADOR");
   do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 0

 } 
  while (!successRead);   //the program will not go further while you are not getting a successful read
ope = temp;
lcd.clear();
lcd.setCursor(3,0);
lcd.print("PROCURANDO");
lcd.setCursor(4,1);
lcd.print("REGISTRO");
delay(1000);
lcd.clear();
lcd.setCursor(4,0);
lcd.print("REGISTRO");
lcd.setCursor(4,1);
lcd.print(ope);
delay(1000);



 
}


void placa(){

  cartaoplaca = 0;
  code = 0;
  // Print a message to the LCD.
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("PASSE O CARTAO");
  lcd.setCursor(2,1);
  lcd.print("DO VEICULO");
  do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 0

 } 
  while (!successRead);   //the program will not go further while you are not getting a successful read
pla = temp;
lcd.clear();
lcd.setCursor(3,0);
lcd.print("PROCURANDO");
lcd.setCursor(4,1);
lcd.print("REGISTRO");
delay(1000);
lcd.clear();
lcd.setCursor(4,0);
lcd.print("REGISTRO");
lcd.setCursor(4,1);
lcd.print(pla);
delay(1000);
 

 
  
}

///////////////////////////////////////// Monta a ID da TAG ///////////////////////////////////
uint8_t getID() {
  temp = "";
  // Getting ready for Reading PICCs0
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    //Serial.print(readCard[i], HEX);
    conc = String(readCard[i], HEX);
    temp = temp += conc;
  }

  //Serial.println("");
  temp.toUpperCase();
  //Serial.print(temp);
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void interruptPin()
{
    rpm = rpm + 1;
 
}
