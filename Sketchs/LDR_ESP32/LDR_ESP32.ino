<<<<<<< HEAD

#define Light  2


void setup() {
  
  Serial.begin(115200);
  delay(1000);
}

void loop() {
int val = analogRead(Light);
Serial.println(val);
delay(100);
}
=======

#define Light  2


void setup() {
  
  Serial.begin(115200);
  delay(1000);
}

void loop() {
int val = analogRead(Light);
Serial.println(val);
delay(100);
}
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
