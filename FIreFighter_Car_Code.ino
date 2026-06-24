// Jay Shree Ram 
// Automatic and Bluetooth control Fire-fighter Robot 
// Sumit Gupta

#include <Servo.h>         // library for servo motor

#define RM1 2     // Right Motor 1 
#define RM2 3     // Right Motor 2
#define LM1 4     // left Motor 1
#define LM2 5     // Left Motor 2
#define ENA 9      // Right Motor Speed control 
#define ENB 10   // Left Motor speed control
#define Pump 6    // For Water supply 
#define Buzzer 13   // Horn
#define Front_Light 12 // Head Light
#define Back_Light 7   // Back Light

Servo S1;
int pos=90;
bool Auto=false;
int sweepDir=1;
unsigned long lastSweepTime = 0;

#define Right_Flame_Sensor A0 // Right Flame Sensor
#define Forword_Flame_Sensor A1 // Forword Flame Sensor
#define Left_Flame_Sensor A2 // Left Flame Sensor

// creat a Array for speed control
    int val[10]={0,50,75,100,125,150,175,200,225,255};
    int speed=9;  // default speed

void setup()
{
  Serial.begin(9600);  // Serial Moniter 
  Serial.println("F0\tF1\tF2\tBl\tspeed");  // for Column

  // define how to use digital PIN
  pinMode(RM1,OUTPUT);
  pinMode(RM2,OUTPUT);
  pinMode(LM1,OUTPUT);
  pinMode(LM2,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(Pump,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(Front_Light,OUTPUT);
  pinMode(Back_Light,OUTPUT);

  S1.attach(11); //servo Pin
  S1.write(pos);
  stopAll();
}

void loop()
{
  int F0=analogRead(Right_Flame_Sensor);
  Serial.print(F0);
  Serial.print("\t");
  int F1=analogRead(Forword_Flame_Sensor);
  Serial.print(F1);
  Serial.print("\t");
  int F2=analogRead(Left_Flame_Sensor);
  Serial.print(F2);
  Serial.print("\t");

  if (Auto==true)
   {
     if(millis()-lastSweepTime>15)
     {
       lastSweepTime=millis();
       pos=pos+sweepDir;

       if(pos>=120)
       {
          pos=120;
          sweepDir=-1;
        }
       if(pos<=60)
       {
         pos=60;
         sweepDir=1;
        }
       S1.write(pos);
      }
    }

  if (Serial.available())
  {
    char Bl=Serial.read();
    Serial.print(Bl);
    Serial.print("\t");
    
    if (Bl>='0' && Bl<='9')
    {
      speed=Bl-'0';
    }
    Serial.println(speed);
    
    switch (Bl)
    {
      case 'F':
      analogWrite(ENA,val[speed]);
      analogWrite(ENB,val[speed]);
      Forword();
      break;
      case 'B':
      analogWrite(ENA,val[speed]);
      analogWrite(ENB,val[speed]);
      Backword();
      break;
      case 'R':
       if(Auto==false)
        {
          pos=pos+10;
          if (pos>180) pos=180;
          S1.write(pos);
        } 
       else
       {
         analogWrite(ENA,val[speed]);
         analogWrite(ENB,val[speed]);
         Right();
        }
      break;
      case 'L':
       if(Auto==false)
        {
          pos=pos-10;
          if (pos<0) pos=0;
          S1.write(pos);
        } 
       else
       {
         analogWrite(ENA,val[speed]);
         analogWrite(ENB,val[speed]);
         Left();
        }
      break;
      case 'G':
      analogWrite(ENA,val[speed<9?speed+1:speed]);
      analogWrite(ENB,val[speed]);
      Forword();
      break;
      case 'I':
      analogWrite(ENA,val[speed]);
      analogWrite(ENB,val[speed<9?speed+1:speed]);
      Forword();
      break;
      case 'H':
      analogWrite(ENA,val[speed<9?speed+1:speed]);
      analogWrite(ENB,val[speed]);
      Backword();
      break;
      case 'J':
      analogWrite(ENA,val[speed]);
      analogWrite(ENB,val[speed<9?speed+1:speed]);
      Backword();
      break;
      case 'S':
        stop();
        break;
      case 'P':
        stopAll();
        break;
      case 'K':
      Auto=true;
      sweepDir=1;
      break;
      case 'k':
      Auto=false;
      break;
    }
    if (Bl=='V') digitalWrite(Buzzer,HIGH);
    if (Bl=='v') digitalWrite(Buzzer,LOW);
    if (Bl=='P') digitalWrite(Pump,HIGH);
    if (Bl=='p') digitalWrite(Pump,LOW);
    if (Bl=='W') digitalWrite(Front_Light,HIGH);
    if (Bl=='w') digitalWrite(Front_Light,LOW);
    if (Bl=='U') digitalWrite(Back_Light,HIGH);
    if (Bl=='u') digitalWrite(Back_Light,LOW);
  }

  else
  {
    Serial.print("A");
    Serial.print("\t");
    Serial.println(speed);

   if(F0<=100 && F1>=900 && F2>=900)      // Right Flame Sense
   {
    analogWrite(ENA,val[speed]); analogWrite(ENB,val[speed]);
    Right();
    delay(800);
    stop();
   }
   else if(F0>=1000 && F1>=1000 && F2>=1000) 
   {
    stop();
   }
   else if(900<=F0 && F1<=900 && 80<F1 && 900<=F2 && F1<F0 && F1<F2) //Forword Flame Sense
   {
    analogWrite(ENA,val[speed]); analogWrite(ENB,val[speed]);
    Forword();
    delay(800);
    stop();
   }
   else if(F0>=900 && F1>=900 && F2<=100)  // Left Flame Sensor
   {
    analogWrite(ENA,val[speed]); analogWrite(ENB,val[speed]);
    Left();
    delay(800);
    stop();
   }
   else if (F0>1000 && 100>F1 && F1>=0 && F2>1000) // stop car & start Pump with Servo
   {
    stop();
    digitalWrite(Pump,HIGH);
    servo();
    digitalWrite(Pump,LOW);
    analogWrite(ENA,val[speed]); analogWrite(ENB,val[speed]);
    Backword();
    delay(800);
    stop();
   }
   if(F0<=900 || F1<=900 || F2<=900)  // Flame Detaction Warning
   {
     digitalWrite(Buzzer,HIGH);
     delay(200);
     digitalWrite(Buzzer,LOW);
    }
  }
delay(100); 
}

void Forword()
{
  digitalWrite(RM1,HIGH); digitalWrite(RM2,LOW);
  digitalWrite(LM1,HIGH); digitalWrite(LM2,LOW);
}
void Backword()
{
  digitalWrite(RM1,LOW); digitalWrite(RM2,HIGH);
  digitalWrite(LM1,LOW); digitalWrite(LM2,HIGH);
}
void Right()
{
  digitalWrite(RM1,LOW); digitalWrite(RM2,HIGH);
  digitalWrite(LM1,HIGH); digitalWrite(LM2,LOW);
}
void Left()
{
  digitalWrite(RM1,HIGH); digitalWrite(RM2,LOW);
  digitalWrite(LM1,LOW); digitalWrite(LM2,HIGH);
}
void stopAll()
{
  digitalWrite(RM1,LOW); digitalWrite(RM2,LOW);
  digitalWrite(LM1,LOW); digitalWrite(LM2,LOW);
  digitalWrite(Pump,LOW);digitalWrite(Buzzer,LOW);
  digitalWrite(Front_Light,LOW); digitalWrite(Back_Light,LOW);
  analogWrite(ENA,0); analogWrite(ENB,0);
  Auto=false;
  pos=90;
  S1.write(pos);
} 

void stop()
{
  digitalWrite(RM1,LOW); digitalWrite(RM2,LOW);
  digitalWrite(LM1,LOW); digitalWrite(LM2,LOW);
  analogWrite(ENA,0); analogWrite(ENB,0);
  delay(200);
}

void servo()
{
  int Ram=0;
  while (Ram<3)
  {
    for(pos=90; pos<=120; pos+=1)
    {
      S1.write(pos);
      delay(15);
    }
    for(pos=120; pos>=60; pos-=1)
    {
      S1.write(pos);
      delay(15);
    }
    for(pos=60; pos<=90; pos+=1)
    {
      S1.write(pos);
      delay(15); 
    }
    Ram++;
  }
}