/***INSTANCIANDO OBJETOS***********************************************************************************************************************************/

// Configuraçoes e Endereço de Internet WIFI
WiFiServer server                (8090);
//Variável para armazenar a solicitação HTTP
String header;
// Hora Atual
unsigned long currentTime = millis();
// Vez anterior
unsigned long previousTime = 0; 
//Defina o tempo limite em milissegundos 
const long timeoutTime = 2000;




// Reset Wifi Manager
WiFiManager wifiManager;




 


// Status para controle de Led
int Status_Led = LOW; 
