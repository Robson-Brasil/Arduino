//Sensor de Ré
//Por Robson Brasil
//Versão 1

const int TRIG = 3, ECHO = 2, buzzer = 4;
unsigned int intervalo, distancia;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  distancia = sensor_morcego(TRIG,ECHO); // Chamada da função de leitura.
  Serial.println(distancia);
  if (distancia <= 10) { // Condicional para leituras inferiores a 10cm.
    digitalWrite(10,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(5,HIGH);
    tone(buzzer, 1750);
  }
  else if (distancia > 60) { // Condicional para leituras superiores a 60cm.
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,LOW);
      digitalWrite(7,LOW);
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);
      noTone(buzzer);
  } 
  else { // Condicional para leitura intermediarias.
    if (distancia <= 20) { 
      digitalWrite(10,LOW);
      digitalWrite(9,HIGH);
      digitalWrite(8,HIGH);
      digitalWrite(7,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(5,HIGH);
      intervalo = 100;
    }
    else if (distancia <= 30) {
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,HIGH);
      digitalWrite(7,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(5,HIGH);
      intervalo = 150;
    }
    else if (distancia <= 40) {
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,LOW);
      digitalWrite(7,HIGH);
      digitalWrite(6,HIGH);
      digitalWrite(5,HIGH);
      intervalo = 200;
    }
    else if (distancia <= 50) {
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,LOW);
      digitalWrite(7,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(5,HIGH);
      intervalo = 250;
    }
    else if (distancia <= 60) {
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,LOW);
      digitalWrite(7,LOW);
      digitalWrite(6,LOW);
      digitalWrite(5,HIGH);
      intervalo = 300;
    }
    tone(buzzer,1750);
    delay(intervalo);
    noTone(buzzer);
    delay(intervalo);
  }
}

int sensor_morcego(int pinotrig,int pinoecho){ // Função para leitura do sensor
  digitalWrite(pinotrig,LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig,HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig,LOW);

  return pulseIn(pinoecho,HIGH)/59;
}