//Totalmente creado por LPZ ELECTRONICS para: YouTube (www.youtube.com/antoniolr95), Taringa y http://antoniolr95.blogspot.com


#define LED 9     //El LED esta conectado en el pin 9 
#define LDR 0     //El LDR esta conectador en el pin A0

int luz = 0;       
int valor_sensor = 0;       
int valor_limite = 490;  //Este valor hara que el LED cambie de estado a una determinada luminosidad (podeis con distintos valores para ajustar la sensibilidad)


void setup() {      //Configuracion de los pin como entrada o salida 
  Serial.begin(9600);    
  pinMode(LED,OUTPUT);
  pinMode(LDR,INPUT);
}

void loop ()  //Configuracion de los valores del LDR
{
  valor_sensor = analogRead(LDR); 
  luz = (5.0 * valor_sensor * 100.0)/1024.0; //Para entender esta formula visitar: http://programarfacil.com/podcast/48-sensor-de-temperatura-en-arduino/
  Serial.print(luz);  
  Serial.println(" Luz");             
  delay(300);                       
  
  if (luz <= valor_limite)   //Si el valor de luz es menor o igual que el valor limite
  {
    digitalWrite (LED, LOW);  //El led se apaga
  }
  if (luz > valor_limite)   //Si es mayor que el valor limite
  {
    digitalWrite (LED, HIGH);  //El led se eniende
  }
}
