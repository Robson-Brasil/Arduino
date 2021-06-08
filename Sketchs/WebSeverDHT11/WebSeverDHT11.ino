#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <DHT.h>

#define DHTTYPE DHT11 // DHT 11
#define Rele1 0
#define Rele2 15
#define Rele3 2
#define Rele4 4
#define LED   5

bool Estado_Do_Rele1 = LOW;
bool Estado_Do_Rele2 = HIGH;
bool Estado_Do_Rele3 = LOW;
bool Estado_Do_Rele4 = LOW;
bool Estado_Do_Led = LOW;

// DHT Sensor
const int DHTPin = 14;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
 
// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
 
// Client variables
char linebuf[80];
int charcount=0;

// substitua os dados entre parênteses com o nome da Rede e a senha desejados
const char* ssid     = "MOTOROLA-A7CE9";
const char* password = "3104e43ece65b0885fa9";

WiFiServer server(80);

void setup()
{
    dht.begin();
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode
    delay(10);
    pinMode(Rele1, OUTPUT);
    pinMode(Rele2, INPUT);
    pinMode(Rele3, OUTPUT);
    pinMode(Rele4, OUTPUT);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print(" Configurando... ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Conectado.");
    Serial.println("O Endereço IP Da Rede : ");
    Serial.println(WiFi.localIP());
    
    server.begin();
    {
Serial.println("Failed to read from DHT sensor!");
}


}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("Novo Cliente.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

            /* Se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
            esse é o fim da solicitação HTTP do cliente, então envie uma resposta: */
          if (currentLine.length() == 0) {
            /* Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
            e um tipo de conteúdo para que o cliente saiba o que está por vir e, em seguida, uma linha em branco: */
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println();
            client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv=\"refresh\" content=\"2\"></head>");
            client.println("<body><h1><p>ESP32 DHT11 example</h1><p>");
 
            client.println(celsiusTemp);
            client.println("*C</p><p>");
            client.println(fahrenheitTemp);
            client.println("*F</p><p>");
            client.println(humidityTemp);
            client.println("%</p>");
            client.println("</body></html>");
            break;

             // Configuração de estilo do site
            client.print("<style type=\"text/css\"> a{margin: 50px 50px; background: #000000; color: #ffffff; text-decoration: none; padding: 1% 20% 1%; border-radius: 10px; font-size: 8.0em;}</style>");
             // O conteúdo do cabeçalho HTTP
            client.print("<a href=\"/T1\">Rele 1. </a> </br></br></br>");
            client.print("<a href=\"/T2\">Rele 2. </a> </br></br></br>");
            client.print("<a href=\"/T3\">Rele 3. </a> </br></br></br>");
            client.print("<a href=\"/T4\">Rele 4. </a> </br></br></br>");
            client.print("<a href=\"/H\">LED. </a> </br></br></br></br>");
            
            
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

if (currentLine.endsWith("GET /H")) {
 
if (Estado_Do_Led == LOW) {
digitalWrite(LED, HIGH);
Estado_Do_Led = HIGH;
} else {
digitalWrite(LED, LOW);
Estado_Do_Led = LOW;
}
}        
if (currentLine.endsWith("GET /T1")) {
 
if (Estado_Do_Rele1 == LOW) {
digitalWrite(Rele1, HIGH);
Estado_Do_Rele1 = HIGH;
} else {
digitalWrite(Rele1, LOW);
Estado_Do_Rele1 = LOW;
}
}
if (currentLine.endsWith("GET /T2")) {
 
if (Estado_Do_Rele2 == LOW) {
digitalWrite(Rele2, HIGH);
Estado_Do_Rele2 = HIGH;
} else {
digitalWrite(Rele2, LOW);
Estado_Do_Rele2 = LOW;
}
}
if (currentLine.endsWith("GET /T3")) {
 
if (Estado_Do_Rele3 == LOW) {
digitalWrite(Rele3, HIGH);
Estado_Do_Rele3 = HIGH;
} else {
digitalWrite(Rele3, LOW);
Estado_Do_Rele3 = LOW;
}
}
if (currentLine.endsWith("GET /T4")) {
 
if (Estado_Do_Rele4 == LOW) {
digitalWrite(Rele4, HIGH);
Estado_Do_Rele4 = HIGH;
} else {
digitalWrite(Rele4, LOW);
Estado_Do_Rele4 = LOW;
}
}
}
}
    // Termina a conexão com o cliente
    client.stop();
    Serial.println("Cliente Desconectado.");
  }
}
