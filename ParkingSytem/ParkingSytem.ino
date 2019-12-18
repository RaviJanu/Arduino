 //!!AUM!!
//INIT...
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define Enterence_Sensor  A0
#define SLOT_1_Sensor     A1
#define SLOT_2_Sensor     A2
#define EXIT_Sensor       A3

#define Gate_Motor_1      8
#define Gate_Motor_2      9
#define Camera_Motor_1    10
#define Camera_Motor_2    11
#define Key_1             12
#define Key_2             13


void setup() 
{  // put your setup code here, to run once:
  pinMode(Enterence_Sensor,INPUT);
  pinMode(SLOT_1_Sensor,INPUT);
  pinMode(SLOT_2_Sensor,INPUT);
  pinMode(EXIT_Sensor,INPUT); 
  pinMode(Key_1,INPUT_PULLUP);
  pinMode(Key_2,INPUT_PULLUP);
  pinMode(Gate_Motor_1,OUTPUT);
  pinMode(Gate_Motor_2,OUTPUT);
  pinMode(Camera_Motor_1,OUTPUT);
  pinMode(Camera_Motor_2,OUTPUT);
  
  Serial.begin(9600);

  digitalWrite(Gate_Motor_1, LOW);
  digitalWrite(Gate_Motor_2, LOW);
  digitalWrite(Camera_Motor_1, LOW);
  digitalWrite(Camera_Motor_2, LOW);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("CAR PARKING SYS ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(5000);
}

char Camera_Event_flag = 0,Door_flag = 0,Sloat1_Flag = 0,Sloat2_Flag = 0;
int base_delay = 0;

void loop() 
{ 
  if(Serial.available())
  { char r = Serial.read();
    if(r == 'Q')  
    { Door_flag = 1;
    }
  }

  if(digitalRead(Enterence_Sensor) == 1)
  { if(Camera_Event_flag == 0)
    { Camera_Event_flag = 1;
      Serial.write('E');
      digitalWrite(Camera_Motor_1,HIGH);
      digitalWrite(Camera_Motor_2,LOW);
      delay(1000);
      digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,LOW);
    }
  }
  if(digitalRead(EXIT_Sensor) == 1)
  { if(Camera_Event_flag == 0)
    { Camera_Event_flag = 2;
      Serial.write('X');
      digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,HIGH);
      delay(1000);
      digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,LOW);
    }
  }
  if((digitalRead(Enterence_Sensor) == 0)&&(digitalRead(EXIT_Sensor) == 0))
  { if(Camera_Event_flag == 1)
    { digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,HIGH);
      delay(1000);
      digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,LOW);
      Camera_Event_flag = 0;
    }
    if(Camera_Event_flag == 2)
    { digitalWrite(Camera_Motor_1,HIGH);
      digitalWrite(Camera_Motor_2,LOW);
      delay(1000);
      digitalWrite(Camera_Motor_1,LOW);
      digitalWrite(Camera_Motor_2,LOW);
      Camera_Event_flag = 0;
    }
    
  }
  
  if(digitalRead(Key_1) == 0) { digitalWrite(Camera_Motor_1,HIGH);
                                digitalWrite(Camera_Motor_2,LOW);
                                delay(100);
                                digitalWrite(Camera_Motor_1,LOW);
                                digitalWrite(Camera_Motor_2,LOW);
                              }

  if((digitalRead(Key_2) == 0)&&(Door_flag == 1))
  {   Door_flag = 0;
      lcd.setCursor(0, 0);
      lcd.print(" PAYMENT DONE   ");  
      lcd.setCursor(0, 1);
      lcd.print("   DOOR OPEN    ");
      delay(1000);
      digitalWrite(Gate_Motor_1,HIGH);
      digitalWrite(Gate_Motor_2,LOW);
      delay(1000);
      digitalWrite(Gate_Motor_1,LOW);
      digitalWrite(Gate_Motor_2,LOW);  
      delay(10000);
      digitalWrite(Gate_Motor_1,LOW);
      digitalWrite(Gate_Motor_2,HIGH);
      delay(1000);
      digitalWrite(Gate_Motor_1,LOW);
      digitalWrite(Gate_Motor_2,LOW);  
  }

  delay(1);
  base_delay++;
  if(base_delay < 2000)   return; 
  base_delay = 0;

  if(digitalRead(SLOT_1_Sensor) == 1)
  {   lcd.setCursor(0, 0);
      lcd.print("SLOT 1 : FULL   ");
      if(Sloat1_Flag == 0)  
      { Sloat1_Flag = 1;
        Serial.write('A');
      }  
  }
  else
  {   lcd.setCursor(0, 0);
      lcd.print("SLOT 1 : EMPTY  ");  
      if(Sloat1_Flag == 1)
      { Sloat1_Flag = 0;
        Serial.write('a');
      }
  }
 
  if(digitalRead(SLOT_2_Sensor) == 1)
  {   lcd.setCursor(0, 1);
      lcd.print("SLOT 2 : FULL   ");
      if(Sloat2_Flag == 0)  
      { Sloat2_Flag = 1;
        Serial.write('B');
      }  
  }
  else
  {   lcd.setCursor(0, 1);
      lcd.print("SLOT 2 : EMPTY  ");
      if(Sloat2_Flag == 1)  
      { Sloat2_Flag = 0;
        Serial.write('b'); 
      }  
  }
                                  
}























