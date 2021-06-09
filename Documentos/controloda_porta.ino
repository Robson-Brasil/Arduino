int tec0 = 0;
int gajo [7] = {0, 0, 0, 0, 0, 0, 0};
int tes0 [7] = {0, 0, 0, 0, 0, 0, 0};
static int tric = 19;
static int vet0 = 27;
static int vet1 = 14;
static int vet2 = 18;
static int ori0 = 32;
static int ori1 = 33;
static int ori2 = 25;
static int ori3 = 26;
int dit = 0;
int tec00 = 0;
bool c = 0;
int t = 0;
bool e = 0;
int p = 0;
bool a = 0;

#include <LiquidCrystal.h>;


LiquidCrystal lcd(2, 4, 19, 23, 5, 13);


void lertec() {
  delay(10);
  digitalWrite(vet0, HIGH);
  digitalWrite(vet1, LOW);
  digitalWrite(vet2, LOW);
  if (digitalRead(ori0) == 1) {
    tec0 = 1;
    lcd.setCursor(0, 0);
    lcd.print(tec0);
  }
  else {
    if (digitalRead(ori1) == 1) {
      tec0 = 4;
      lcd.setCursor(0, 0);
      lcd.print(tec0);
    }
    else {
      if (digitalRead(ori2) == 1) {
        tec0 = 7;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
      else if (digitalRead(ori3) == 1) {
        e = 1;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
    }
  }
  delay(10);
  digitalWrite(vet0, LOW);
  digitalWrite(vet1, HIGH);
  digitalWrite(vet2, LOW);
  if (digitalRead(ori0) == 1) {
    tec0 = 2;
    lcd.setCursor(0, 0);
    lcd.print(tec0);
  }
  else {
    if (digitalRead(ori1) == 1) {
      tec0 = 5;
      lcd.setCursor(0, 0);
      lcd.print(tec0);
    }
    else {
      if (digitalRead(ori2) == 1) {
        tec0 = 8;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
      else if (digitalRead(ori3) == 1) {
        tec0 = 0;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
    }
  }
  delay(10);
  digitalWrite(vet0, LOW);
  digitalWrite(vet1, LOW);
  digitalWrite(vet2, HIGH);
  if (digitalRead(ori0) == 1) {
    tec0 = 3;
    lcd.setCursor(0, 0);
    lcd.print(tec0);
  }
  else {
    if (digitalRead(ori1) == 1) {
      tec0 = 6;
      lcd.setCursor(0, 0);
      lcd.print(tec0);
    }
    else {
      if (digitalRead(ori2) == 1) {
        tec0 = 9;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
      else if (digitalRead(ori3) == 1) {
        c = 1;
        lcd.setCursor(0, 0);
        lcd.print(tec0);
      }
    }
  }
}
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(vet1, OUTPUT);
  pinMode(vet0, OUTPUT);
  pinMode(vet2, OUTPUT);
  pinMode(tric, OUTPUT);
  pinMode(ori0, INPUT);
  pinMode(ori1, INPUT);
  pinMode(ori2, INPUT);
  pinMode(ori3, INPUT);
}

void loop() {
  for (int f = 0; f != 7; f++) {
    lertec();
    t = t + 1;
    lcd.setCursor(0, 0);
    lcd.print("   bem  Vindo   ");
    lcd.setCursor(0, 1);
    lcd.print("Ao C.lab 0000000");
    //0110472
    lcd.setCursor(9, 1);
    for (int i = 0; i != 7; i++) {
      lcd.print(gajo[i]);
    }
    lcd.cursor();
    lcd.setCursor(9 + dit, 1);
    gajo [dit] = tec0;
    if (tec0 != tec00 || t == 50)
    {
      t = 0;
      dit = dit + 1;
      if (dit == 8)dit = 0;
    }
    tec00 = tec0;
  }
  if (c == 1) {
    for (int f = 0; f != 7; f++)
    {
      gajo [f] = 0;

    }
    dit = 0;
    c = 0;
  }
  if (e == 1)
  {
    e = 0;
    a = 1;
    for (int f = 0; f != 7; f++)
    {
      tes0[f] = gajo[f]+48;
    }
  }
  if (a == 1)
  {
    Serial.print(1);
    for (int f = 0; f != 7; f++)
    {
      Serial.print(tes0[f]-48);
    }
    Serial.println();
    for (int i = 0; i < 7; i++)
    {
      if (Serial.read() == tes0[i] && p == i)
      {
        p = p + 1;
      }
      else
      {
        p = 0;
      }
    }
    if (p == 7)
    {
      p = 0;
      Serial.println("a");
      a=0;
    }
  }
  Serial1.println(a);
}


//lcd.setCursor(0,0);
//lcd.print("Humidade   ");
// lcd.setCursor(0,1);
// lcd.print("Temperatura");
