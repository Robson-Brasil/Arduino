// Teste sensor de chuva 

int sensorChuva = 14;         // Pino A0 do módulo conectado no GPIO 14 do esp32 
int valorLimiteChuva = 2000; // Valor numérico da tensão de comparação do sensor / valor máximo = 1024 
int led = 12; 
bool chuva; 

void setup() 
{
  pinMode(sensorChuva, INPUT); 
  pinMode(led, OUTPUT); 
  Serial.begin(115200); 
}

void sensorDeChuva() 
{
  int valorSensorChuva = analogRead(sensorChuva); 
  Serial.print("Sensor de chuva = "); 
  Serial.print(valorSensorChuva); 
  if(valorSensorChuva < valorLimiteChuva) 
  {
    Serial.println(" => Esta chovendo"); 
    chuva = 1; 
  } 
  else 
  {
    Serial.println(" => O tempo esta seco"); 
    chuva = 0; 
  }
}

void loop() 
{
  for(int i = 1; i < 120; i++) 
  {
    sensorDeChuva(); 
    chuva == 1 ? digitalWrite(led, HIGH) : digitalWrite(led, LOW); 
    Serial.print(i*5); 
    Serial.println(" segundos"); 
    Serial.println(); 
    delay(5000); 
  }
}
