#define DIR_PIN 4
#define STEP_PIN 7
//#define ENABLE_PIN 10 // define os pinos do driver a4988

#define sensorA 5 // define os pinos dos sensores
#define sensorB 6 
int obstaculoA = HIGH;
int obstaculoB = HIGH; //define os sensores como nivel alto

const int stepsPerRevolution = 200; //definição de passos por giro do motor

typedef enum { ESTADO_PARADO,
               ESTADO_VAI,
               ESTADO_VOLTA } estados;

estados estado = ESTADO_PARADO;
estados estadoAnterior = estado;

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  //pinMode(ENABLE_PIN, OUTPUT);

  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
}

void loop(){
  switch(estado){
    case ESTADO_PARADO:
    while(estado == ESTADO_PARADO){
      obstaculoA = digitalRead(sensorA);
      obstaculoB = digitalRead(sensorB);

      if((obstaculoA == LOW) && (estadoAnterior != ESTADO_VAI)){
        estado = ESTADO_VAI;
      }
      if((obstaculoB == LOW) && (estadoAnterior != ESTADO_VOLTA)){
        estado = ESTADO_VOLTA;
      }
   }
   break;

   case ESTADO_VAI:
   digitalWrite(DIR_PIN, HIGH);

   while (estado == ESTADO_VAI){
    for(int n = 0; n < stepsPerRevolution; n++){
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(2000);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(2000);
    }
    estado = ESTADO_PARADO;
    estadoAnterior = ESTADO_VAI;
    
   }
   break;

   case ESTADO_VOLTA:
   digitalWrite(DIR_PIN, LOW);

   while (estado  == ESTADO_VOLTA){

    for(int n = 0; n < stepsPerRevolution; n++){
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(2000);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(2000);
    }
    estado = ESTADO_PARADO;
    estadoAnterior = ESTADO_VOLTA;
   }
   break;
  }
}
