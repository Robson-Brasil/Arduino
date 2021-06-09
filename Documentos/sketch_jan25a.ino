

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(100);
  }

void loop() {

String recebido = "W|1234|10";   //SerialBT.readStringUntil('#');
String literal;
String rede;
String caixa;

literal=recebido.substring(0,1); 
rede=recebido.substring(2,6);
caixa=recebido.substring(7,9); 

Serial.println (literal);
Serial.println (rede);
Serial.println (caixa);

int _rede=rede.toInt(); //tranformando uma string de números em inteiro
int _caixa=caixa.toInt(); //tranformando uma string de números em inteiro

// As duas linha a seguir é para teste confirmação da int
_rede++; // incrementa o número contido na _rede para teste na linha abaixo
Serial.print("A soma de _rede + 1 = "); 
Serial.println(_rede);// confirmação que a variável _rede é uma int

while(1);
 
}
