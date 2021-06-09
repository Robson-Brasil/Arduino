int x = 4; //to control the rotation
int y = 5; //to control the rotation
int pot = A0;
float angle = 0, angleReaded = 0;
boolean flag = true; //True go up (right), false go down (left)
boolean readOK = true; //True try te read the Serial
int desired_angle = -1;
String codigo = "";

void setup() {
  pinMode(x, OUTPUT); //connected to the IN1 pin of the motor driver
  pinMode(y, OUTPUT); //connected to the IN2 pin of the motor driver
}

void turnRight() { //to the motor driver
  digitalWrite(x, 130);
  digitalWrite(y, LOW);
}

void turnLeft() { //same
  digitalWrite(x, LOW);
  digitalWrite(y, 130);
}

void turnOff() {
  digitalWrite(x, LOW);
  digitalWrite(y, LOW);
}

void loop() {

  angleReaded = analogRead(pot);
  angle = map(angleReaded, 0, 1023, 0, 270);
    
  if (readOK) {

    if (Serial.available() > 0)
    
    {
       Serial.print(angle);
      /* As Victor said, Arduino can only read 1 character in the serial window so we
          need something to fix it. In the Arduino documentation there is a loop available
          to achieve this goal. The loop doesnt break unless there is a new line (\n)
      */
      desired_angle = -1;
      codigo = "";
      while (Serial.available() > 0) {
        char ch = Serial.read();
        if (isDigit(ch)) { //Filtrado de entrada solo deja numeros 0 al 9
          codigo += ch;
        }
      }
      desired_angle = codigo.toInt();


      
      if(desired_angle != (-1)){
        readOK = false;
        Serial.print("El angulo deseado es : ");
        Serial.print(desired_angle);
      }
    }
  } else {

  
    if (angle > 200)
      {
      turnLeft();
      delay(1000);
      turnOff();
      delay(500);
      }
      else
      turnRight();

    Serial.println(angle);

   if (flag) // Girar a la derecha 0->270
    {
      if (angle < 200)
        turnLeft();
      else
        flag = false;
    }
    else
    {
      if (angle > 10)
        turnRight();
      else
        flag = true;
    }
    

    if(angle>desired_angle){
       turnRight();
    }else if (angle<desired_angle){
        turnLeft();
    }else{
        turnOff();
        readOK=true;
    }


    
  }
}
