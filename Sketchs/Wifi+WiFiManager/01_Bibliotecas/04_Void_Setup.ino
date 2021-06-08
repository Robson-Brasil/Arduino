
void setup() {

  Serial.begin(115200);

  Serial.println("Configurar Rede Wifi");




/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //  SETUP -- wifi -- ////////////////////////////
  wifiManager.autoConnect("FK_SOLUTIONS", "12345678");

  //wifiManager.resetSettings ();   // Apaga todas redes Salvas do Wifi

  delay(150);
  WiFi.begin();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("------------------------------------------------------------");
    Serial.println("Conectado IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("------------------------------------------------------------");

    delay(150);
    server.begin();
  }




/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //*** SETUP -- Definição dos Pinos -- //////////
  delay(50);

  
  pinMode(Led,   OUTPUT);

  // Iniciam em LOW -> Desligados
  digitalWrite(Led,   LOW);
  








/////////////////////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("INICIOU **************************");

}
