void loop () {




  /////////////////////////////////////////////////////////////////////////////////////////////////////////


  WiFiClient client = server.available();    // Ouça os clientes que chegam
   if (client) {                             // Se um novo cliente se conectar
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Novo Cliente");          // imprimir uma mensagem na porta serial
    String currentLine = "";                // faça uma string para conter os dados de entrada do cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // faz um loop enquanto o cliente está conectado
      currentTime = millis();
      if (client.available()) {             // se houver bytes para ler do cliente
        char c = client.read();             // leia um byte, então
        Serial.write(c);                    // imprima no monitor serial
        header += c;
        if (c == '\n') {                    // se o byte é um caractere de nova linha
          // se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
          // esse é o fim da solicitação HTTP do cliente, então envie uma resposta:
          if (currentLine.length() == 0) {
            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir, em seguida, uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Verifica o caracter enviado pelo html (GET)********************************

            //**************************************************
            if (header.indexOf("GET /L1") >= 0) {
              Status_Led =! Status_Led;
              digitalWrite(Led, Status_Led);
            }

            // DEFINIÇÕES - CORPO - CÓDIGO PÁGINA HTML *****************************************************

            //**************************************************
            // Exibir a página da web em HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title> WiFi Lora </title>");                     // Titulo do topo da Pagina

            //**************************************************
            // CSS para estilizar os botões e Pagina
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #F0FFF0;}");
            client.println(".button { background-color: #4CAF50; border-radius: 15px; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {  background-color: #555555;}</style></head>");

            //**************************************************
            // Título da página da web
            client.println("<body>    <h1>WiFi LoRa</h1> <h3> Web Server</h3>");

            // Atualização do HTML ************************************************
            if (Status_Led) {
              client.print("<a href=\"/L1\"><button class=\"button\">LED-ON</button></a>");
            } else {
              client.print("<a href=\"/L1\"><button class=\"button button2\">LED-OFF</button></a>");
            }
           
            client.println("<P>");

            client.println("Wifi LoRa a Solucao em Automacao");
            client.println("<P>");


            client.println("</body></html>");

            client.println();                     // A resposta HTTP termina com outra linha em branco:
            break;                                // interromper o loop while:
          } else {
            currentLine = "";                     // se você recebeu uma nova linha, limpe currentLine:
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    // fecha a conexão:
    client.stop();
    Serial.println("Cliente Desconectado.");
    Serial.println("------------------------------------------------------------");
  }


}
