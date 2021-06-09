char command;
String string;
const int ledPin1 =  13;      
const int ledPin2 =  12;
const int ledPin3 =  11;
const int ledPin4 =  10;

void setup() {
Serial.begin(9600);

pinMode(ledPin1, OUTPUT);    
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
pinMode(ledPin4, OUTPUT);
}
  void loop()  {
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
    
if(string == "z"){      
     digitalWrite(13,0);
     digitalWrite(12,0);
     digitalWrite(11,0);
     digitalWrite(10,0);
    delay(1);    
  }
}
 

