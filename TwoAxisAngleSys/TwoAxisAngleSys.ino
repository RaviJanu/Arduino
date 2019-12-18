//!!OM!!
#include <LiquidCrystal.h>
// RS-7  ; EN-6 ; D4-5    ;  D5-4   ; D6-3    ; D7-2
LiquidCrystal lcd(22, 24, 26, 28, 30, 32);
#include <Servo.h>
Servo ARM_MOTOR_1,ARM_MOTOR_2;
#define SERVO_MOTOR_1   3
#define SERVO_MOTOR_2   5

#define KEY_1   0xE1
#define KEY_2   0xE2
#define KEY_3   0xE4
#define KEY_A   0xE8
#define KEY_4   0xD1
#define KEY_5   0xD2
#define KEY_6   0xD4
#define KEY_B   0xD8
#define KEY_7   0xB1
#define KEY_8   0xB2
#define KEY_9   0xB4
#define KEY_C   0xB8
#define KEY_S   0x71
#define KEY_0   0x72
#define KEY_H   0x74
#define KEY_D   0x78

const byte Key_Row[4] = {A4,A5,A6,A7};
const byte Key_Col[4] = {A0,A1,A2,A3};
byte Key_Val = 0;
byte key_col_sel = 0;
byte avoid_sence_cnt = 0,avoid_sence_flag = 0;

byte Torque = 0,Angulation = 0;
unsigned int base_delay = 0;

void setup() 
{ char i = 0;
  for(i = 0;i<4;i++)
  { pinMode(Key_Col[i],OUTPUT);
    digitalWrite(Key_Col[i],HIGH);
    pinMode(Key_Row[i],INPUT_PULLUP);
  }
  Serial.begin(9600);
  ARM_MOTOR_1.attach(SERVO_MOTOR_1);
  ARM_MOTOR_2.attach(SERVO_MOTOR_2);
  
  ARM_MOTOR_1.write(90); 
  ARM_MOTOR_2.write(90); 
  
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("   TWO AXIS     ");
  lcd.setCursor(0, 1);
  lcd.print("ANGULATION SYSTM");
  delay(5000);
}

byte Set_parameter = 0;
void loop() 
{ key_action();

  delay(1);

  base_delay++;
  if(base_delay < 500) return;
  base_delay = 0;

  
  lcd.setCursor(0, 0);
  lcd.print("TORQUE: "+String(Torque)+"     ");
  lcd.setCursor(0, 1);
  lcd.print("ANGULATION: "+String(Angulation)+"    ");
  if(Set_parameter == 1)  
  { delay(500);
    lcd.setCursor(0, 0);
    lcd.print("                 ");
  }
  if(Set_parameter == 2)  
  { delay(500);
    lcd.setCursor(0, 1);
    lcd.print("                 ");
  }
}


void key_action(void)
{ key_sence();
  if((Key_Val & 0x0f) == 0)     return;

  byte key_num;
  
  switch(Key_Val)
  { case KEY_1:   key_num = 1;
    break;
    case KEY_2:   key_num = 2;
    break;
    case KEY_3:   key_num = 3;
    break;
    case KEY_4:   key_num = 4;
    break;
    case KEY_5:   key_num = 5;
    break;
    case KEY_6:   key_num = 6;
    break;
    case KEY_7:   key_num = 7;
    break;
    case KEY_8:   key_num = 8;
    break;
    case KEY_9:   key_num = 9;
    break;
    case KEY_0:   key_num = 0;
    break;
    case KEY_A:   key_num = 10;
    break;
    case KEY_B:   key_num = 11;
    break;
    case KEY_C:   key_num = 12;
    break;
    case KEY_D:   key_num = 13;
    break;
    case KEY_S:   key_num = 14;
    break;
    case KEY_H:   key_num = 15;
    break;
  }
  Serial.println(key_num);
  if(key_num == 10) Set_parameter = 0;
  if(key_num == 11) Set_parameter = 1;
  if(key_num == 12) Set_parameter = 2;
  if(key_num == 13) ServoMotorSide();

  if(key_num > 9) return;
  if(Set_parameter == 1)
  { Torque = Torque*10 + key_num;
    if(Torque > 90) Torque = 0;
  }
  if(Set_parameter == 2)
  { Angulation = Angulation*10 + key_num;
    if(Angulation > 90) Angulation = 0;
  }
  
}


void ServoMotorSide(void)
{ byte TorqueServo = 0,AngleServo = 0;
  Set_parameter = 0;
  TorqueServo = Torque + 45;
  ARM_MOTOR_1.write(TorqueServo); 
  AngleServo = Angulation + 45;
  ARM_MOTOR_2.write(AngleServo); 
  Serial.println("Torque: "+String(TorqueServo)+"Angle: "+String(AngleServo));
  lcd.setCursor(0, 0);
  lcd.print("TORQUE: "+String(TorqueServo)+" *   ");
  lcd.setCursor(0, 1);
  lcd.print("ANGULATION: "+String(AngleServo)+" *   ");
  delay(1000);
}

void key_sence(void)
{   char i = 0;
    byte bit_tbl[4] = {0xFE,0xFD,0xFB,0xF7};

    Key_Val = 0x00;
    
   if(avoid_sence_flag == 1)
   {  avoid_sence_cnt++;
      if(avoid_sence_cnt < 250)  return;
      avoid_sence_cnt = 0; 
      avoid_sence_flag = 0;
   }

   for(i = 0;i<4;i++)
   {  digitalWrite(Key_Col[i],HIGH);
   } 
   digitalWrite(Key_Col[key_col_sel],LOW);

   Key_Val =  bit_tbl[key_col_sel] << 4;    
   
   for(i = 0;i<4;i++)
   {  if(digitalRead(Key_Row[i]) == 0)
      { Key_Val |= ~bit_tbl[i];
        break;
      }
   }

   if( (Key_Val & 0x0f) != 0x00)
   {  avoid_sence_flag = 1;
   }
   
   key_col_sel++;
   if(key_col_sel > 3)  key_col_sel = 0;
}


