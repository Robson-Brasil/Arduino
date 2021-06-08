
#include <WiFi.h>

const char* ssid     = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";

WiFiServer server(80);

#define ATUADOR1 21
#define ATUADOR2 12
#define ATUADOR3 14
#define ATUADOR4 27
#define ATUADOR5 26
#define ATUADOR6 25
#define ATUADOR7 33
#define ATUADOR8 32
#define ATUADOR9 19
#define ATUADOR10 2


bool Estado_Do_Atuador1 = LOW;
bool Estado_Do_Atuador2 = LOW;
bool Estado_Do_Atuador3 = LOW;
bool Estado_Do_Atuador4 = LOW;
bool Estado_Do_Atuador5 = LOW;
bool Estado_Do_Atuador6 = LOW;
bool Estado_Do_Atuador7 = LOW;
bool Estado_Do_Atuador8 = LOW;
bool Estado_Do_Atuador9 = LOW;
bool Estado_Do_Atuador10 = LOW;


void setup()
{
    Serial.begin(115200);
    pinMode(ATUADOR1, OUTPUT);
    pinMode(ATUADOR2, OUTPUT);
    pinMode(ATUADOR3, OUTPUT);
    pinMode(ATUADOR4, OUTPUT);
    pinMode(ATUADOR5, OUTPUT);
    pinMode(ATUADOR6, OUTPUT);
    pinMode(ATUADOR7, OUTPUT);
    pinMode(ATUADOR8, OUTPUT);
    pinMode(ATUADOR9, OUTPUT);
    pinMode(ATUADOR10, OUTPUT);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Configurando... ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Conectado");
    Serial.println("Endereco de IP: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("Novo Cliente");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Configuração de estilo do site
            client.print("<style type=\"text/css\"> a{margin: 50px 50px; background: #000000; color: #ffffff; text-decoration: none; padding: 1% 20% 1%; border-radius: 10px; font-size: 8.0em;}</style>");
             // O conteúdo do cabeçalho HTTP
            client.print("<a href=\"/A1\">BOTAO 1 </a> </br></br></br>");
            client.print("<a href=\"/A2\">BOTAO 2 </a> </br></br></br>");
            client.print("<a href=\"/A3\">BOTAO 3 </a> </br></br></br>");
            client.print("<a href=\"/A4\">BOTAO 4 </a> </br></br></br>");
            client.print("<a href=\"/A5\">BOTAO 5 </a> </br></br></br>");
            client.print("<a href=\"/B1\">BOTAO 6 </a> </br></br></br>");
            client.print("<a href=\"/B2\">BOTAO 7 </a> </br></br></br>");
            client.print("<a href=\"/B3\">BOTAO 8 </a> </br></br></br>");
            client.print("<a href=\"/B4\">BOTAO 9 </a> </br></br></br>");
            client.print("<a href=\"/B5\">BOTAO10 </a> </br></br></br>");
            
            
             // A resposta HTTP termina com outra linha em branco:
            client.println();
            // Sair do loop while:
            break;
          } else {    // Se você tiver uma nova linha, então limpe a linha atual:
            currentLine = "";
          }
        } else if (c != '\r') {  // Se você tiver qualquer outra coisa além de um caractere de retorno de linha,
          currentLine += c;      // adicioná-lo ao final da linha atual
        }

        // Verifica se o pedido do cliente foi para atualizar algum dos reles (GET /T)

          if (currentLine.endsWith("GET /A1")) {
 
if (Estado_Do_Atuador1 == LOW) {
digitalWrite(ATUADOR1, HIGH);
Estado_Do_Atuador1 = HIGH;
} else {
digitalWrite(ATUADOR1, LOW);
Estado_Do_Atuador1 = LOW;
}
}        
if (currentLine.endsWith("GET /A2")) {
 
if (Estado_Do_Atuador2 == LOW) {
digitalWrite(ATUADOR2, HIGH);
Estado_Do_Atuador2 = HIGH;
} else {
digitalWrite(ATUADOR2, LOW);
Estado_Do_Atuador2 = LOW;
}
}
if (currentLine.endsWith("GET /A3")) {
 
if (Estado_Do_Atuador3 == LOW) {
digitalWrite(ATUADOR3, HIGH);
Estado_Do_Atuador3 = HIGH;
} else {
digitalWrite(ATUADOR3, LOW);
Estado_Do_Atuador3 = LOW;
}
}
if (currentLine.endsWith("GET /A4")) {
 
if (Estado_Do_Atuador4 == LOW) {
digitalWrite(ATUADOR4, HIGH);
Estado_Do_Atuador4 = HIGH;
} else {
digitalWrite(ATUADOR4, LOW);
Estado_Do_Atuador4 = LOW;
}
}
if (currentLine.endsWith("GET /A5")) {
 
if (Estado_Do_Atuador5 == LOW) {
digitalWrite(ATUADOR5, HIGH);
Estado_Do_Atuador5 = HIGH;
} else {
digitalWrite(ATUADOR5, LOW);
Estado_Do_Atuador5 = LOW;
}
}
if (currentLine.endsWith("GET /B1")) {
 
if (Estado_Do_Atuador6 == LOW) {
digitalWrite(ATUADOR6, HIGH);
Estado_Do_Atuador6 = HIGH;
} else {
digitalWrite(ATUADOR6, LOW);
Estado_Do_Atuador6 = LOW;
}
}
if (currentLine.endsWith("GET /B2")) {
 
if (Estado_Do_Atuador7 == LOW) {
digitalWrite(ATUADOR7, HIGH);
Estado_Do_Atuador7 = HIGH;
} else {
digitalWrite(ATUADOR7, LOW);
Estado_Do_Atuador7 = LOW;
}
}
if (currentLine.endsWith("GET /B3")) {
 
if (Estado_Do_Atuador8 == LOW) {
digitalWrite(ATUADOR8, HIGH);
Estado_Do_Atuador8 = HIGH;
} else {
digitalWrite(ATUADOR8, LOW);
Estado_Do_Atuador8 = LOW;
}
}
if (currentLine.endsWith("GET /B4")) {
 
if (Estado_Do_Atuador9 == LOW) {
digitalWrite(ATUADOR9, HIGH);
Estado_Do_Atuador9 = HIGH;
} else {
digitalWrite(ATUADOR9, LOW);
Estado_Do_Atuador9 = LOW;
}
}
if (currentLine.endsWith("GET /B5")) {
 
if (Estado_Do_Atuador10 == LOW) {
digitalWrite(ATUADOR10, HIGH);
Estado_Do_Atuador10 = HIGH;
} else {
digitalWrite(ATUADOR10, LOW);
Estado_Do_Atuador10 = LOW;
}
}
}
    }
    // Termina a conexão com o cliente
    client.stop();
    Serial.println("Cliente Desconectado");
    }
  }
