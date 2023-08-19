#include<TimerOne.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleKalmanFilter.h>
#include<Keypad.h>
LiquidCrystal_I2C lcd(0x27,16,2);
SimpleKalmanFilter loc(2,2,0.01);

  
const byte sohang = 4;
const byte socot = 3;
char hexaKeys[sohang][socot] =
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}, 
  {'*','0','#'},
};
byte pinhang[sohang] = {7,8,9,10};
byte pincot[socot] = {11,12,13};
Keypad customkeypad = Keypad(makeKeymap(hexaKeys), pinhang, pincot, sohang, socot);

int ena = 6;
int in1 = 5;
int in2 = 4;
int cambien = 2;
int led1 = 0;
int led2 = 1;
double xung = 0;  
double Speed = 0; 
char StrMax[4];
char StrMin[4];
int Max;
int Min;
int Begin1 = 0;



void setup() 
{
  
  pinMode(ena,OUTPUT);
  pinMode(cambien,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);

  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  
  lcd.init();
  lcd.backlight();
  
  attachInterrupt(0,Count,FALLING);
  Timer1.initialize(1000000);            
  Timer1.attachInterrupt(Tocdo);
}
void Count()
{
  xung++;
}
void Tocdo()
{
  Speed = (xung/8)*60;
  xung = 0;
}
void loop() 
{
  Start();
  Control();
  char test;
  test = customkeypad.getKey();
  if(test)
  {
    if(test == '*')
    {
      Input(StrMin,StrMax);
      lcd.clear();
    }
  }
  if(Speed >= Min && Speed <= Max)
  {
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
  }
  else
  {
    digitalWrite(led2,HIGH);
    digitalWrite(led1,LOW);
  }
  
  lcd.setCursor(0,0);   lcd.print("TOC DO DONG CO");
  lcd.setCursor(0,1);   lcd.print(Speed);  
  lcd.setCursor(9,1);   lcd.print("V/P");

 


}
void Control()
{
  double value = analogRead(A0);   
  value = loc.updateEstimate(value);
  double output;
  output = map(value,0,1023,0,140);   
  
  analogWrite(ena,output);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  Serial.print("VR: ");
  Serial.println(value);

//  lcd.setCursor(0,0);   lcd.print("VR: ");    lcd.print(value);
//  lcd.setCursor(0,1);   lcd.print("PWM:");    lcd.print(output);
}

void Input(char x[],char y[])
{
  do
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nhap Min: ");
    int r = 10;
    int i = 0;
    while(i < 5)
    {
      char key = customkeypad.getKey();
      if(key)
    {
      if(key == '*')
        {
          break;
        }
      else if(key == '#')
        {
          Del(StrMin);
          i = 0;
          r = 10;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Nhap Min: ");
        }
      else if(i == 4)
        {
          if(key == '*')
            i++;
        }
      else
        {
          lcd.setCursor(r,0); 
          lcd.print(key);
          StrMin[i] = key;              
          r++;
          i++;
        }
    }
    }

    Min = Convert(StrMin);
    
    lcd.setCursor(0,1);
    lcd.print("Nhap Max: ");
    r = 10;
    i = 0;
    while(i < 5)
    { 
      lcd.setCursor(0,0);   lcd.print("Nhap Min: ");   lcd.print(Min);
      char key = customkeypad.getKey();
      if(key)
    {
      if(key == '*')
        {
          break;
        }
      else if(key == '#')
        {
          Del(StrMax);
          i = 0;
          r = 10;
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Nhap Max: ");
        }
      else if(i == 4)
        {
          if(key == '*')
            i++;
        }
      else
        {
          lcd.setCursor(r,1); 
          lcd.print(key);
          StrMax[i] = key;              
          r++;
          i++;
        }
    }
    }

    Max = Convert(StrMax);
    if(Min >= Max)
    {
       lcd.clear();
       lcd.setCursor(0,0); lcd.print("Nhap sai!!!");
       lcd.setCursor(0,1); lcd.print("Hay nhap lai!!!");
       delay(2000);
    }
  }while(Min >= Max);
  lcd.clear();

}
  
int Convert(char a[])
{
    String e;
    int x;
    for(int i=0;i<=3;i++)
      e += a[i];
    x = e.toInt();
    for(int i=0;i<=3;i++)
      a[i] = 0;
    return x;
}
void Del(char a[])
{
    for(int i=0;i<=3;i++)
      a[i] = 0;
}
void Start()
{
   while(Begin1 < 1)
   {
      char Begin2;
      lcd.setCursor(0,0);
      lcd.print("PRESS * TO START");
      Begin2 = customkeypad.getKey();
      if(Begin2)
      {
        if(Begin2 == '*')
        {
          lcd.clear();
          Begin1 = 1;
        }
      }
   }
}
