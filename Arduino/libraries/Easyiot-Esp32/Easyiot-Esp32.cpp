#include <Easyiot-Esp32.h>

const char* host = "app.easyiot.com.br";
const int httpsPort = 443;
WiFiClientSecure client;
String JsonDados;

void Json(String Dados, String Token){

 JsonDados = "{\"payload\": \"";
 JsonDados = JsonDados + Dados + "\"";
 JsonDados = JsonDados + ",\r\n \"token\": \"";
 JsonDados = JsonDados + Token;
 JsonDados = JsonDados + "\"}";
}


String send(String Dados, String Token) {
	char a, b;
	bool c = false;

	String resposta = "";

    Json(Dados, Token);
    // Connectando ao servidor Easyiot
	Serial.print("Conectando a ");
	Serial.println(host);
	if (!client.connect(host, httpsPort)) {
		Serial.println("Conexao Falhou, verifique sua WiFi.");
		Serial.println();
		resposta = "Conexao Falhou, verifique sua WiFi.";
	    return resposta;
	}

	String url = "/api/device-data/in";

	client.print(String("POST ") + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Connection: close\r\n" +
    "Content-Type: application/json\r\n" +
    "Content-Length: " + JsonDados.length() + "\r\n" +
    "\r\n" +
    JsonDados + "\r\n");
    
  while (client.connected()) { //Leitura do Header
    if(client.available()) {

    	b = a;
	    a = client.read();

	    if ((a == 13) && (b == 10)) {
	    	c = true;
	    }
	    if ((a == 13) && (b == 10) && (c = true)) {
	      break;
	    }
	}
  }

  while (client.connected()) { //Leitura da resposta
    if(client.available()) {

	    a = client.read();
	    resposta = resposta + a;
	}
	else{
		client.stop();	
	}
	
  }

  if((resposta.compareTo("\nErro: Dispositivo não encontrado, verifique seu Easy Token") == 0) || (resposta.compareTo("\nErro: Você não possui mais créditos, adquira mais na plataforma EasyIoT") == 0) || (resposta.compareTo("\nErro de servidor") == 0)) {
	 Serial.println(resposta); 
	 Serial.println();
  }
  else {
	  Serial.println("Seus dados foram enviados com sucesso, verifique no painel da plataforma.");
      Serial.println();
	  Serial.print("Resposta do servidor: ");
      Serial.println(resposta);
  	  Serial.println();
  }


  return resposta;

}