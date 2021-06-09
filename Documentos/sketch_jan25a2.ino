

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(100);
  }

void loop() {

String recebido = "Teste|12345678|87654321";   //SerialBT.readStringUntil('#');
String literal;
String rede;
String caixa;
int pos1;
int pos2;
int pos3;



pos1=recebido.indexOf('|');
literal=recebido.substring(0,pos1);
recebido.setCharAt(pos1++,'x');

pos2=recebido.indexOf('|');
rede=recebido.substring(pos1,pos2);
recebido.setCharAt(pos2++,'x');

pos3=recebido.indexOf('|');
caixa=recebido.substring(pos2,pos3);
recebido.setCharAt(pos3++,'x');


Serial.println (literal);
Serial.println (rede);
Serial.println (caixa);



while(1);
 
}
