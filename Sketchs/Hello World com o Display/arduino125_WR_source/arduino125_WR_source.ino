<<<<<<< HEAD
/* ========================================================================================================
 
   Curso de Arduino e AVR 125
   
   WR Kits Channel


   Hello World Display LCD I2C

    
   Autor: Eng. Wagner Rambo  Data: Junho de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits | dailymotion.com/wrkits
   
======================================================================================================== */


// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{
 lcd.begin (20,4);

 
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Hello World");
  lcd.setCursor(0,1);
  lcd.print("Display I2C!!!");
  delay(1000);
  lcd.setBacklight(LOW);
  delay(1000);
  
} //end loop
=======
/* ========================================================================================================
 
   Curso de Arduino e AVR 125
   
   WR Kits Channel


   Hello World Display LCD I2C

    
   Autor: Eng. Wagner Rambo  Data: Junho de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits | dailymotion.com/wrkits
   
======================================================================================================== */


// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{
 lcd.begin (20,4);

 
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Hello World");
  lcd.setCursor(0,1);
  lcd.print("Display I2C!!!");
  delay(1000);
  lcd.setBacklight(LOW);
  delay(1000);
  
} //end loop
>>>>>>> a999f467d0d8ec227170bef607b275053355a84a
