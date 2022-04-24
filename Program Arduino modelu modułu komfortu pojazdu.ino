#include <TimerOne.h>
#include <PCF8574.h>
#include <DallasTemperature.h>
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <DS3231.h>
PCF8574 expander;
PCF8574 expander2;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
OneWire oneWire(A4);
DallasTemperature sensors(&oneWire);

unsigned long aCzas = 0;
unsigned long zCzas = 0;
unsigned long rCzas = 0;
bool went=false;
float t1=20;
float t2=25;
int w=0; //Wentylator
int p=0; // KP
int p2=0;// KP
int l=0;//KL
int l2=0;//KL
int s=0;
int s2=0;
int czujnikSwiatla = 0;

void setup() {
 pinMode(0, INPUT_PULLUP); //Szyba przełącznik dół
 pinMode(1, INPUT_PULLUP);//Szyba przełącznik góra
 pinMode(4, OUTPUT); //Silnik szyby 
 pinMode(5, OUTPUT); //Silnik szyby 
 pinMode(6, OUTPUT); //Sygnał PWM wentylatora
 pinMode(7, OUTPUT); //Kierunkowskaz lewy
 pinMode(8, OUTPUT); //Kierunkowskaz prawy
 pinMode(9, OUTPUT); //Sterowanie PWM silnika szyby
 pinMode(11, OUTPUT); //Światła dzienne
 pinMode(12, OUTPUT); //Światła mijania
 pinMode(13, OUTPUT); //Światła drogowe
 pinMode(A5, INPUT_PULLUP);//Wentylator ON/OFF
 
 Timer1.initialize(100);
 Timer1.pwm(9, 812);
 Serial.begin(9600);
 sensors.begin();
 
 expander.begin(0x20);
  expander.pinMode(0, OUTPUT); //Lusterko silnik 1
  expander.pinMode(1, OUTPUT); //Lusterko silnik 1
  expander.pinMode(2, OUTPUT); //Lusterko silnik 2
  expander.pinMode(3, OUTPUT); //Lusterko silnik 2
  expander.pinMode(7, OUTPUT); //Wentylator
  expander.pinMode(6, OUTPUT); //Wentylator
  expander.pinMode(4, INPUT_PULLUP);//Temp +
  expander.pinMode(5, INPUT_PULLUP);//Temp -
 expander2.begin(0x23); 
  expander2.pinMode(2, INPUT_PULLUP); //Światła auto
  expander2.pinMode(3, INPUT_PULLUP); //Swiatła manual
 
  lcd.begin(16,2);   
  lcd.backlight();
}

void loop() {
////////////////////////////////////////////// Lusterko ///////// //////////////////////////////////////
if (analogRead(A2) > 60 and analogRead(A2) < 180)
{ 
  expander.digitalWrite(1, HIGH);
}
else 
{
  expander.digitalWrite(1, LOW);
}
if ( analogRead(A2) > 200 and analogRead(A2) < 400) 
{
  expander.digitalWrite(0, HIGH);
}
else 
{
  expander.digitalWrite(0, LOW);
}
if (analogRead(A2) > 450 and analogRead(A2) < 650)
{
  expander.digitalWrite(3, HIGH);  
}
else 
{
  expander.digitalWrite(3, LOW);
}
if (analogRead(A2) > 800 and analogRead(A2) < 1100)
{
  expander.digitalWrite(2, HIGH);
}
else 
{
  expander.digitalWrite(2, LOW);
}
//////////////////////////////////////////////// Szyba ///////////////////////////////////////////////////
if (digitalRead(0) == LOW)
{
   digitalWrite(4, HIGH);
}
else
{
  digitalWrite(4, LOW);
}
if (digitalRead(1) == LOW)
{
  digitalWrite(5, HIGH);
}
else
{
  digitalWrite(5, LOW);
}
//////////////////////////////////////////////// Kierunki Prawy //////////////////////////////////////////
if (analogRead(A0) > 600 and analogRead(A0) < 1000 and p2==0)
{
  delay(250);
  p=1;
  
  }
  if (p==1)
  {
   digitalWrite(8, HIGH);
   delay(500);
   digitalWrite(8, LOW);
   delay(500);
    p=2;
  }
  

    if (p==2 and analogRead(A0) < 100)
    {
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      p=3;
       
    }
    if (p==3 and analogRead(A0) < 100)
    {
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
       
    }
    
    if (analogRead(A0) > 600 and analogRead(A0) < 1000)
    {
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500); 
      p=0;
      p2++;
    } 
    else
    {
      p=0;
      p2=0;
    }
/////////////////////////////////////////////// Kierunki Lewy ///////////////////////////////////////////
if (analogRead(A0) > 100 and analogRead(A0) < 600 and l2==0)
{
  delay(250);
  l=1;
  
  }
  if (l==1)
  {
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
  delay(500);
    l=2;
  }
  

    if (l==2 and analogRead(A0) < 100)
    {
      digitalWrite(7, HIGH);
      delay(500);
      digitalWrite(7, LOW);
      delay(500);
      l=3;
       
    }
    if (l==3 and analogRead(A0) < 100)
    {
      digitalWrite(7, HIGH);
      delay(500);
      digitalWrite(7, LOW);
      delay(500);
       
    }
    
    if (analogRead(A0) > 100 and analogRead(A0) < 600)
    {
      digitalWrite(7, HIGH);
      delay(500);
      digitalWrite(7, LOW);
      delay(500); 
      l=0;
      l2++;
    } 
    else
    {
      l=0;
      l2=0;
    }
    
//////////////////////////////////////////////// Światła /////////////////////////////////////////////////
czujnikSwiatla = analogRead(A3);
if (expander2.digitalRead(2) == HIGH and expander2.digitalRead(3) == HIGH) //Światła wyłączone
{
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  if (analogRead(A1) > 100 and analogRead(A1) < 600)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
}
if (expander2.digitalRead(2) == LOW and expander2.digitalRead(3) == HIGH) //Światła auto
{
  if (analogRead(A3) > 100 and s == 0)
  {
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
   
  }
  else
  {
    if (s == 0)
    {
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    }
    else
    {
      digitalWrite(12, LOW);
    }
  }
  if (analogRead(A1))
  {
  if (analogRead(A1) > 100 and analogRead(A1) < 600)
  {
    digitalWrite(13, HIGH);
  }
  if (analogRead(A1) > 700 and analogRead(A1) < 800 and analogRead(A3) < 100)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    s=1;
    
  }
  }
  else
  {
    digitalWrite(13, LOW);
    s=0;
   
  }
}

if (expander2.digitalRead(2) == LOW and expander2.digitalRead(3) == LOW) //Światła manual
{
  if (s2 == 0)
    {
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    }
    else
    {
      digitalWrite(12, LOW);
    }
if (analogRead(A1))
  {
  if (analogRead(A1) > 100 and analogRead(A1) < 600)
  {
    digitalWrite(13, HIGH);
  }
  if (analogRead(A1) > 700 and analogRead(A1) < 800)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    s2=1;
    
  }
  }
  else
  {
    digitalWrite(13, LOW);
   s2=0;
   
  }
}
//////////////////////////////////////////////// Wentylator //////////////////////////////////////////////
if (digitalRead(A5) == LOW)
  {
    delay(500);
    went=!went;
  }
if (went)
  {
if (expander.digitalRead(4) == LOW and t1<30)
  {
    t1=t1+0.5;
  }
  if (expander.digitalRead(5) == LOW and t1>15)
  {
    t1=t1-0.5;
  }
aCzas = millis();
rCzas = aCzas - zCzas;
  if (rCzas >= 2000UL)
  {
  sensors.requestTemperatures();
  t2=sensors.getTempCByIndex(0);
  zCzas = aCzas;
  }
  lcd.setCursor(0,0);
  lcd.print("T.z.:");
  lcd.print(t2,1);
  lcd.print(" Bieg.W");
  lcd.setCursor(0,1);
  lcd.print("T.n.:");
  lcd.print(t1,1);
  lcd.setCursor(12,1);
  lcd.print(w);
  lcd.noCursor();
  
  if (t2-t1>1 or t1-t2>1)
  {
    w=1;
    expander.digitalWrite(7, HIGH);
    expander.digitalWrite(6, LOW);
    analogWrite(6, 100);
  }
  if (t2-t1>5 or t1-t2>5)
  {
    w=2;
    analogWrite(6, 150);
    expander.digitalWrite(7, HIGH);
    expander.digitalWrite(6, LOW);
  }
  if (t2-t1>10 or t1-t2>10)
  {
    w=3;
    analogWrite(6, 200);
    expander.digitalWrite(7, HIGH);
    expander.digitalWrite(6, LOW);
  }
  if (t2-t1>15 or t1-t2>15)
  {
    w=4;
    analogWrite(6, 255);
    expander.digitalWrite(7, HIGH);
    expander.digitalWrite(6, LOW);
  }
  }
  else
  {
    expander.digitalWrite(7, LOW);
    lcd.setCursor(0,0);
    lcd.print("Wentylator      ");
    lcd.setCursor(0,1);
    lcd.print("Wylaczony       ");
  }
 }
