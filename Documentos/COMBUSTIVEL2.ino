
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define SOM 7
#define RST_PIN   9
#define SS_PIN    10
#define relepin 8
#define DS1307_ADDRESS 0x68
File myFile;
LiquidCrystal_I2C lcd(0x27,16,2);
MFRC522 mfrc522(SS_PIN, RST_PIN);
byte zero = 0x00; 
byte sel = 0;
byte c1 = 0;
byte c2 = 0;
byte c3 = 0;
byte status = 0;
String cconteudo = "";
String usuario = "";
String nome = "";
char caractere;
char carac;
byte x = 0; 
byte vezes = 3;
uint8_t successRead;
byte readCard[4];
String temp; 
String ope = "";
String dados = "";
String comp = "";
volatile unsigned int rpm;
byte litros = 0;


void setup() {
// Desabilitando portas que não estão em uso  
pinMode(0,OUTPUT);  
digitalWrite(0,LOW);
pinMode(1,OUTPUT);  
digitalWrite(1,LOW);
pinMode(3,OUTPUT);  
digitalWrite(3,LOW);
pinMode(5,OUTPUT);  
digitalWrite(5,LOW);
pinMode(6,OUTPUT);  
digitalWrite(6,LOW);
pinMode(A0,OUTPUT);  
digitalWrite(A0,LOW);
//pinMode(A1,OUTPUT);  
//digitalWrite(A1,LOW);
//pinMode(A2,OUTPUT);  
//digitalWrite(A2,LOW);
//pinMode(A3,OUTPUT);  
//digitalWrite(A3,LOW);

Wire.begin();
byte com = 0;
pinMode(SOM, OUTPUT);
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
pinMode(relepin, OUTPUT);
lcd.init();
lcd.backlight();
lcd.clear(); 
lcd.setCursor(3,0);
lcd.print(F("INICIANDO"));
lcd.setCursor(4,1);
lcd.print(F("SISTEMA"));
delay(1500); 
if (SD.begin(4)) {
com = 1;
!SD.begin(4);
}
else {
com = 0;
}  
do {

if(com == 0){  
lcd.clear();  
lcd.setCursor(3,0);
lcd.print(F("ESTADO  DE"));
lcd.setCursor(2,1);
lcd.print(F("XXX ERRO XXX"));
//tone(SOM,700);
digitalWrite(SOM, HIGH);
delay(100);
//noTone(SOM);
delay(200);
}
} 
  while (com == 0);   //the program will not go further while you are not getting a successful read
}
void loop() {
 
usuario = "";
nome = "";
comp = "";
operador();
delay(100);
dados = "";
comp = usuario;
dados.concat(usuario);
dados.concat("|");
dados.concat(nome);
dados.concat("|");
usuario = "";
nome = "";
placa();
delay(100);
comp.concat(usuario);
dados.concat(usuario);
dados.concat("|");
dados.concat(nome);
usuario = "";
nome = "";
permissao();
if (status == 1){
  lcd.clear();  
  lcd.setCursor(3,0);
  lcd.print(F("INICIANDO"));
  lcd.setCursor(1,1);
  lcd.print(F("ABASTECIMENTO"));
  for(int j=0; j<vezes; j++)
  {
//  tone(SOM,700);
  digitalWrite(SOM, HIGH);
  delay(100);
 // noTone(SOM);
  delay(200);
  }
  digitalWrite(relepin, HIGH);
  litros = 0;
  rpm = 0;
 do{
 attachInterrupt(0, interruptPin, RISING); 
 attachInterrupt(1, blink, RISING);   
 delay(500);
 litros = digitalRead(relepin);
 if (litros == 1){
 lcd.clear();  
 lcd.setCursor(1,0);
 lcd.print(rpm);
 lcd.setCursor(10,0);
 lcd.print(F("LITROS"));
 lcd.setCursor(2,1);
 lcd.print(F("ABASTECIDOS")); 
 delay(500); 
 } 
 }
 while (litros == 1);
 dados.concat("|");
 dados.concat(rpm);
 if(myFile = SD.open("dac-reg.txt", FILE_WRITE))
 {
 myFile.print(dados);
 myFile.close();
 lcd.clear();  
 lcd.setCursor(3,0);
 lcd.print(F("TERMINO DO"));
 lcd.setCursor(2,1);
 lcd.print(F("ABASTECIMENTO"));
 delay(1000);
 lcd.clear();  
 lcd.setCursor(0,0);
 lcd.print(F("TOTAL DE LITROS:"));
 lcd.setCursor(5,1);
 lcd.print(rpm);
 delay(2000);
 }
 else
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("FALHA AO GRAVAR"));
  lcd.setCursor(4,1);
  lcd.print(F("REGISTROS"));
  delay(5000);
  for(int j=0; j<vezes; j++)
  {
//  tone(SOM,1100);
  digitalWrite(SOM, HIGH);
  delay(500);
 // noTone(SOM);
  delay(100);
  }
  lcd.clear();  
 lcd.setCursor(0,0);
 lcd.print(F("TOTAL DE LITROS:"));
 lcd.setCursor(5,1);
 lcd.print(rpm);
 delay(10000);
  }

} // fecha status

} // fim do loop
///////////////////////////////////////// VERIFICACAO DA PERMISSAO ///////////////////////////////////
void permissao(){
  status = 0;  
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("AGUARDANDO"));
  lcd.setCursor(0,1);
  lcd.print(F("CONFIRMAR  DADOS"));
  delay(500);
//// PROCURA POR CODIGO NO CARTÃO SD ////
c1 = 0;
c2 = 0;
c3 = 0;
myFile = SD.open("dac-adm.txt");
if (myFile) {
while (myFile.available() && (c2 == 0)) {
caractere = 0;
while (myFile.available() && (caractere != ';') && (caractere != 13) && (c1 == 0)) {
caractere = myFile.read();
if ((caractere != ';') && (caractere != 13)) {
usuario.concat(caractere);
}
}
usuario.trim();
if ((comp == usuario ) && (c1 == 0) ) {
status = 1;
c1 = 1;
c2 = 1;
c3 = 1;
}
else {
usuario = "";
status = 0;
}
}
}
else {
lcd.clear();
lcd.setCursor(4,0);
lcd.print(F("FALHA DE"));
lcd.setCursor(3,1);
lcd.print(F("CONEXAO"));
}
if (status != 1) {
lcd.clear();
lcd.setCursor(2,0);
lcd.print(F("ABASTECIMENTO"));
lcd.setCursor(2,1);
lcd.print(F("NAO LIBERADO"));
delay(1000);
for(int j=0; j<vezes; j++)
  {
//  tone(SOM,1100);
  digitalWrite(SOM, HIGH);
  delay(500);
//  noTone(SOM);
  delay(100);
  }
}
else  {
delay(1000);
lcd.clear();
lcd.setCursor(2,0);
lcd.print(F("ABASTECIMENTO"));
lcd.setCursor(4,1);
lcd.print(F("AUTORIZADO"));
delay(1000);  
}
myFile.close();
}
///////////////////////////////////////// INDENTICACAO OPERADOR ///////////////////////////////////
void operador(){
  do {
  status = 0;  
  ope = "";
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print(F("PASSE O CARTAO"));
  lcd.setCursor(2,1);
  lcd.print(F("DO OPERADOR"));
  do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 0
  } 
  while (!successRead);   //the program will not go further while you are not getting a successful read
  ope = temp;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("PROCURANDO"));
  lcd.setCursor(4,1);
  lcd.print(F("REGISTRO"));
  delay(200);
//// PROCURA POR CODIGO NO CARTÃO SD ////
c1 = 0;
c2 = 0;
c3 = 0;
myFile = SD.open("dac-ope.txt");
if (myFile) {
while (myFile.available() && (c2 == 0)) {
caractere = 0;
while (myFile.available() && (caractere != ';') && (caractere != 13) && (c1 == 0)) {
caractere = myFile.read();
if ((caractere != ';') && (caractere != 13)) {
usuario.concat(caractere);
}
}
usuario.trim();
if ((ope == usuario ) && (c1 == 0) ) {
status = 1;
c1 = 1;
c3 = 1;
}
else {
usuario = "";
status = 0;
}
if (c3 == 1) {
nome = "";  
carac = 0;
while (myFile.available() && (carac != 13) && (c3 == 1)) {
carac = myFile.read();
if ((carac != 13) && (carac != ';')) {
nome.concat(carac);
}
delay(10);
}
nome.trim();
if (c3 == 1) {
c3 = 0;
c2 = 1;
sel = 1;
}
}
}
}
else {
lcd.clear();
lcd.setCursor(4,0);
lcd.print(F("FALHA DE"));
lcd.setCursor(3,1);
lcd.print(F("CONEXAO"));
delay(2000);
}
if (status != 1) {
lcd.clear();
lcd.setCursor(2,0);
lcd.print(F("OPERADOR NAO"));
lcd.setCursor(3,1);
lcd.print(F("CADASTRADO"));
delay(1000);
lcd.clear();
lcd.setCursor(1,0);
lcd.print(F("XX CADASTRO XX"));
lcd.setCursor(4,1);
lcd.print(ope);
delay(5000);
for(int j=0; j<vezes; j++)
  {
//  tone(SOM,1100);
  digitalWrite(SOM, HIGH);
  delay(500);
//  noTone(SOM);
  delay(100);
  }
}
else  {
delay(200);
lcd.clear();
lcd.setCursor(4,0);
lcd.print(F("OPERADOR"));
lcd.setCursor(1,1);
lcd.print(nome);
delay(1000);  
}
myFile.close();
}
while (status != 1);
}

///////////////////////////////////////// IDENTIFICA O VEICULO ///////////////////////////////////
void placa(){
  
  ope = "";
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print(F("PASSE O CARTAO"));
  lcd.setCursor(3,1);
  lcd.print(F("DO VEICULO"));
  do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 0
  } 
  while (!successRead);   //the program will not go further while you are not getting a successful read
  ope = temp;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("PROCURANDO"));
  lcd.setCursor(4,1);
  lcd.print(F("REGISTRO"));
  delay(500);
//// PROCURA POR CODIGO NO CARTÃO SD ////
c1 = 0;
c2 = 0;
c3 = 0;
myFile = SD.open("dac-pla.txt");
if (myFile) {
while (myFile.available() && (c2 == 0)) {
caractere = 0;
while (myFile.available() && (caractere != ';') && (caractere != 13) && (c1 == 0)) {
caractere = myFile.read();
if ((caractere != ';') && (caractere != 13)) {
usuario.concat(caractere);
}
}
usuario.trim();
if ((ope == usuario ) && (c1 == 0) ) {
status = 1;
c1 = 1;
c3 = 1;
}
else {
usuario = "";
status = 0;
}
if (c3 == 1) {
nome = "";  
carac = 0;
while (myFile.available() && (carac != 13) && (c3 == 1)) {
carac = myFile.read();
if ((carac != 13) && (carac != ';')) {
nome.concat(carac);
}
delay(10);
}
nome.trim();
if (c3 == 1) {
c3 = 0;
c2 = 1;
sel = 1;
}
}
}
}
else {
lcd.clear();
lcd.setCursor(4,0);
lcd.print(F("FALHA DE"));
lcd.setCursor(3,1);
lcd.print(F("CONEXAO"));
}
if (status != 1) {
lcd.clear();
lcd.setCursor(2,0);
lcd.print(F("VEICULO  NAO"));
lcd.setCursor(3,1);
lcd.print(F("CADASTRADO"));
delay(1000);
lcd.clear();
lcd.setCursor(1,0);
lcd.print(F("XX CADASTRO XX"));
lcd.setCursor(4,1);
lcd.print(ope);
delay(2500);
for(int j=0; j<vezes; j++)
  {
//  tone(SOM,1100);
  digitalWrite(SOM, HIGH);
  delay(500);
 // noTone(SOM);
  delay(100);
  }
}
else  {
delay(1000);
lcd.clear();
lcd.setCursor(1,0);
lcd.print(F("VEICULO  PLACA"));
lcd.setCursor(4,1);
lcd.print(nome);
delay(1000);  
}
myFile.close();

}

///////////////////////////////////////// Monta a ID da TAG ///////////////////////////////////
uint8_t getID() {
  temp = "";
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }

   for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     temp.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     temp.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  temp.toUpperCase();;
  mfrc522.PICC_HaltA();
  return 1;
}
///////////////////////////////////////// CONTA PULSOS ///////////////////////////////////
void interruptPin()
{
    rpm = rpm + 1;
}

///////////////////////////////////////// BOTAO PARA ENCERAR ABASTECIMENTO ///////////////////////////////////
void blink()
{
static unsigned long lastMillis = 0;

unsigned long newMillis = millis();

if(newMillis - lastMillis<100){

}

else{
   digitalWrite(relepin, LOW);
}
}
/////////////////////////////////////
byte ConverteparaDecimal(byte val)  
{ 
  //Converte de BCD para decimal
  return ( (val/16*10) + (val%16) );
}
