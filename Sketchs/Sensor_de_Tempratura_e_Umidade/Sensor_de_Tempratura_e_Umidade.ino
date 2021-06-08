// Programa : Sensor de Temperatura e Umidade
// Autor : Robson Brasil
// Versão : 2
// Última Modificação : 01/01/2020 

// Inclui a biblioteca DHT que possui as funções dos sensores do tipo DHT
#include "DHT.h"

const int pino_dht = 6; // pino onde o sensor DHT está conectado
float temperatura; // variável para armazenar o valor de temperatura
float umidade; // variável para armazenar o valor de umidade
DHT dht(pino_dht, DHT11); // define o pino e o tipo de DHT

void setup() {
  // Inicia e configura a Serial
  Serial.begin(9600); // 9600bps

  dht.begin(); // inicializa o sensor DHT
}

void loop() {
  // Aguarda alguns segundos entre uma leitura e outra
  delay(2000); // 2 segundos (Datasheet)

  // A leitura da temperatura ou umidade pode levar 250ms
  // O atraso do sensor pode chegar a 2 segundos
  temperatura = dht.readTemperature(); // lê a temperatura em Celsius
  umidade = dht.readHumidity(); // lê a umidade

  // Se ocorreu alguma falha durante a leitura
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha na leitura do Sensor DHT!");
  }
  else { // Se não
    // Imprime o valor de temperatura  
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" *C ");
    
    Serial.print("\t"); // tabulação
  
    // Imprime o valor de umidade
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.print(" %\t"); 
    
    Serial.println(); // nova linha
  }
}
