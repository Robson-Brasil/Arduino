#include <SPI.h> //biblioteca para comunicação do barramento SPI
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h> //biblioteca responsável pela comunicação com o módulo RFID-RC522

#define SS_PIN 21
#define RST_PIN 22
#define CartaoLiberado " 56 B9 EC 1F"
#define AcionaBuzzer 10
#define BeepsLiberado 2
#define BeepsNegado 4


//esse objeto 'chave' é utilizado para autenticação
MFRC522::MIFARE_Key key;
//código de status de retorno da autenticação
MFRC522::StatusCode status;
 
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

LiquidCrystal_I2C lcd (0x27, 2, 16);
 
char st[20];
void mensageminicial();
String UID= "";
byte letra;
 
void setup() 
{
  pinMode(AcionaBuzzer,OUTPUT);
  Serial.begin(9600);   // Inicia a serial
   SPI.begin();          // Inicia  SPI bus
   mfrc522.PCD_Init();   // Inicia MFRC522
   lcd.backlight();      // Ativa a Luz de Fundo do LCD
   lcd.begin();
   StandardMessage();
}
 
void loop() 
{
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  getUID();  //separa a string com o UID para ser testada

  if (UID == CartaoLiberado) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    //Mostra mensagem de acesso liberado  e aciona buzzer
    lcd.print("Acesso Liberado!");
    BuzzerBeeps(BeepsLiberado);
    delay(2000);
    StandardMessage();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    //Mostra mensagem de acesso negado e aciona buzzer
    lcd.print("Acesso Negado!");
    BuzzerBeeps(BeepsNegado);
    delay(2000);
    StandardMessage();
  }
  
  delay(1000);
} 

void StandardMessage()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Pronto para");  
  lcd.setCursor(0,1);
  lcd.print(" Leitura!");  
}

void getUID()
{
  Serial.print("UID da tag : ");
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     if( mfrc522.uid.uidByte[i] < 0x10)
       UID.concat(" 0");
     else
       UID.concat(" ");
      
    // conteudo.concat( String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     UID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  UID.toUpperCase();
  Serial.println(" ");
}

void BuzzerBeeps(int NumeroBeeps)
{
  int i;
  for(i=0; i<NumeroBeeps; i++)
  {
    digitalWrite(AcionaBuzzer, HIGH);
    delay(500);
    digitalWrite(AcionaBuzzer, LOW);
    delay(500);
  }
}
