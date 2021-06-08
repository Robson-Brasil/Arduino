#define BLYNK_USE_DIRECT_CONNECT

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleSerialBLE.h>

char command;
String string;
const int ledPin1 =  13;      
const int ledPin2 =  12;
const int ledPin3 =  11;
const int ledPin4 =  10;
const int ledPin5 =  9;    
const int ledPin6 =  8;
const int ledPin7 =  7;
const int ledPin8 =  6;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  DebugSerial.println("Waiting for connections...");

  // Blynk will work through Serial
  // 9600 is for HC-06. For HC-05 default speed is 38400
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  {
Serial.begin(9600);
pinMode(ledPin1, OUTPUT);    
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
pinMode(ledPin4, OUTPUT);
pinMode(ledPin5, OUTPUT);    
pinMode(ledPin6, OUTPUT);
pinMode(ledPin7, OUTPUT);
pinMode(ledPin8, OUTPUT);
}
}

void loop()
{
  Blynk.run();
  {
 if (Serial.available() > 0) 
    {string = "";}
    while(Serial.available() > 0){
    command = ((byte)Serial.read());
      
  if(command == ':'){
        break;
    }
  else {
        string += command;
    }
     delay(1);
    }
    if(string == "A"){
     digitalWrite(13,1);
     delay(1);   
    }
    if(string == "B"){
     digitalWrite(12,1);
     delay(1);   
    }
    if(string == "C"){
     digitalWrite(11,1);
     delay(1);   
    }
    if(string == "D"){
     digitalWrite(10,1);
     delay(1);   
    }
   if(string == "E"){
     digitalWrite(9,1);
     delay(1);   
    }
   if(string == "F"){
     digitalWrite(8,1);
     delay(1);   
    }
   if(string == "G"){
     digitalWrite(7,1);
     delay(1);   
    }
   if(string == "H"){
     digitalWrite(6,1);
     delay(1);   
    }
///////////////
    
  if(string == "a"){
    digitalWrite(13,0);
    delay(1);    
     }
    if(string == "b"){      
    digitalWrite(12,0);
    delay(1);    
     }
    if(string == "c"){
    digitalWrite(11,0);
    delay(1);    
      }
    if(string == "d"){      
     digitalWrite(10,0);
     delay(1);    
      }
   if(string == "e"){
    digitalWrite(9,0);
    delay(1);    
     }
   if(string == "f"){      
    digitalWrite(8,0);
    delay(1);    
     }
   if(string == "g"){
    digitalWrite(7,0);
    delay(1);    
      }
  if(string == "h"){      
     digitalWrite(6,0);
    delay(1);    
      }
if(string == "z")
    {      
     digitalWrite(13,0);
     digitalWrite(12,0);
     digitalWrite(11,0);
     digitalWrite(10,0);
     digitalWrite(9,0);
     digitalWrite(8,0);
     digitalWrite(7,0);
     digitalWrite(6,0);
     delay(1);    
  }
}
}
