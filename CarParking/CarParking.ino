//!!AUM!!
// IR Sensors, Motors, LCD, 4X4 Keypad, IOT
#include <LiquidCrystal.h>
// RS-7  ; EN-6 ; D4-5    ;  D5-4   ; D6-3    ; D7-2
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


#define IR_SLOT_1             40
#define IR_SLOT_2             41
#define IR_SLOT_3             42
#define IR_SLOT_4             43
#define IR_LIFT_LOW_IN        44
#define IR_LIFT_UP_IN         45
#define IR_LIFT_LOW_EDZ       46
#define IR_LIFT_UP_EDZ        47

#define KEY_1                 A0
#define KEY_2                 A1
#define KEY_3                 A2
#define KEY_4                 A3
#define KEY_5                 A4
#define IR_GATE_IN            A5

#define PARKING_MOTOR_1       22
#define PARKING_MOTOR_2       23
#define GATE_MOTOR_1          24
#define GATE_MOTOR_2          25

#define PARKING_UP            'Q'
#define PARKING_DOWN          'W'
#define PARKING_STOP          'X'
#define GATE_OPEN             'R'
#define GATE_CLOSE            'T'
#define GATE_STOP             'Z'

#define INPUT_DATA_BUFF_SIZE    50
int crnt_match_point = 0;
char browser_LinkID = 0, App_LinkID = 0, Module_LinkID = 0;
char AppContDataSending = 0;
int ContSending_timer = 0;


byte current_motion = 0;
byte Slot_1 = 0, Slot_2 = 0, Slot_3 = 0, Slot_4 = 0, Alcohol_flag = 0, Theft_flag = 0, Fir_flag = 0;
#define MAX_PASSWARD      10
byte Passward_Array[MAX_PASSWARD][5] =
{ {5, 4, 2, 3, 1},
  {1, 3, 2, 5, 4},
  {5, 4, 1, 2, 3},
  {4, 5, 1, 3, 2},
  {3, 2, 5, 4, 1},
  {2, 4, 3, 5, 1},
  {2, 3, 4, 1, 5},
  {1, 5, 3, 2, 4},
  {4, 1, 2, 5, 3},
  {3, 1, 2, 5, 4},
};

void setup()
{ pinMode(PARKING_MOTOR_1, OUTPUT);
  pinMode(PARKING_MOTOR_2, OUTPUT);
  pinMode(GATE_MOTOR_1, OUTPUT);
  pinMode(GATE_MOTOR_2, OUTPUT);
  pinMode(IR_SLOT_1, INPUT);
  pinMode(IR_SLOT_2, INPUT);
  pinMode(IR_SLOT_3, INPUT);
  pinMode(IR_SLOT_4, INPUT);
  pinMode(IR_LIFT_LOW_IN, INPUT);
  pinMode(IR_LIFT_UP_IN, INPUT);
  pinMode(IR_LIFT_LOW_EDZ, INPUT);
  pinMode(IR_LIFT_UP_EDZ, INPUT);
  pinMode(IR_GATE_IN, INPUT);

  pinMode(KEY_1, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);
  pinMode(KEY_5, INPUT_PULLUP);

  PARKING_MOVMENT(GATE_STOP);
  PARKING_MOVMENT(PARKING_STOP);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   TWO LEVEL    ");
  lcd.setCursor(0, 1);
  lcd.print(" SMART PARKING  ");

  Serial.begin(9600);
  Serial1.begin(115200);
  delay(5000);

  if (!AT_test())
  { Serial1.println(F("Check ESP Module and Restart Arduino"));
    lcd.setCursor(0, 1);
    lcd.print("WIFI MODULE ERR ");
  }
  if (!Server_setup())
  { Serial1.println(F("Server Not Setup"));
    lcd.setCursor(0, 1);
    lcd.print("CONNECTION ERR  ");
  }
}


unsigned int  base_ms = 0;
byte          ProccessStep = 0;

void loop()
{ GET_NEW_DATA();
  Motor_Control();
  Left_Mechanisum();

  delay(1);
  base_ms++;
  if (base_ms < 1000)  return;
  base_ms = 0;

  parking_status_display();
  Passward_Logic();

  Send_Continously();
}

#define LIFT_UP       0
#define LIFT_DOWN     1
#define LIFT_STOP     2
char lift_updown = 0;

void Left_Mechanisum(void)
{ if (ProccessStep == 0) return;
  byte low_in_ir = 0, up_in_ir = 0;
  low_in_ir = digitalRead(IR_LIFT_LOW_IN);
  up_in_ir =  digitalRead(IR_LIFT_UP_IN);
  Serial.println("A"+String(low_in_ir)+"B"+String(up_in_ir));
  if ((low_in_ir == 0) && (up_in_ir == 0))   return;

  lcd.setCursor(0, 0);
  lcd.print(" WAIT FOR LIFT  ");
  lcd.setCursor(0, 1);
  if(up_in_ir == 1)                 lcd.print("CALL FROM 2LEVEL");
  if(low_in_ir == 1)                lcd.print("CALL FROM 1LEVEL");
  Serial.println("lift move");
  char direction_flag = 0;
  delay(2000);
  

  while (1)
  { low_in_ir = digitalRead(IR_LIFT_LOW_IN);
    up_in_ir =  digitalRead(IR_LIFT_UP_IN); 
    Serial.println("L"+String(digitalRead(IR_LIFT_LOW_EDZ))+"U"+String(digitalRead(IR_LIFT_UP_EDZ)));
   
    if (low_in_ir == 1)
    { if (digitalRead(IR_LIFT_LOW_EDZ) == 0)
      { lift_updown = 0;
        direction_flag = 4;
      }
      if (digitalRead(IR_LIFT_UP_EDZ) == 0)
      { lift_updown = 2;
        direction_flag = 2;
      }
    }
    if (up_in_ir == 1)
    {
      if (digitalRead(IR_LIFT_LOW_EDZ) == 0)
      { lift_updown = 1;
        direction_flag = 3;
      }
      if (digitalRead(IR_LIFT_UP_EDZ) == 0)
      { lift_updown = 0;
        direction_flag = 1;
      }
    }
    if (lift_updown == 1)    PARKING_MOVMENT(PARKING_UP);
    if (lift_updown == 2)    PARKING_MOVMENT(PARKING_DOWN);
    if ((lift_updown == 0) || (digitalRead(KEY_1) == 0))   break;
    delay(10);
  }

  lcd.setCursor(0, 0);
  lcd.print("TAKE YOUR CAR IN");
  
  PARKING_MOVMENT(PARKING_STOP);
  delay(5000);
  Serial.println("Lift NEXT STEP");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print(" WAIT INSIDE LIFT ");
  
  while (1)
  {  Serial.println("L"+String(digitalRead(IR_LIFT_LOW_EDZ))+"U"+String(digitalRead(IR_LIFT_UP_EDZ)));

    if ((direction_flag == 1) || (direction_flag == 3))
    { if (digitalRead(IR_LIFT_LOW_EDZ) == 0)
      { lift_updown = 0;
      }
      if (digitalRead(IR_LIFT_UP_EDZ) == 0)
      { lift_updown = 2;
      }
    }
    if ((direction_flag == 2) || (direction_flag == 4))
    { if (digitalRead(IR_LIFT_LOW_EDZ) == 0)
      { lift_updown = 1;
      }
      if (digitalRead(IR_LIFT_UP_EDZ) == 0)
      { lift_updown = 0;
      }
    }
    if (lift_updown == 1)    PARKING_MOVMENT(PARKING_UP);
    if (lift_updown == 2)    PARKING_MOVMENT(PARKING_DOWN);
    if ((lift_updown == 0) || (digitalRead(KEY_1) == 0))   break;    
 
    delay(10);
  }
  PARKING_MOVMENT(PARKING_STOP);
  Serial.println("Lift STOP");  
  lcd.setCursor(0, 0);
  lcd.print("TAKE YourCar OUT");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(3000);

}


void Motor_Control(void)
{ if (ProccessStep != 0)   return;

  if (!digitalRead(KEY_1))   PARKING_MOVMENT(GATE_OPEN);
  if (!digitalRead(KEY_2))   PARKING_MOVMENT(GATE_CLOSE);

  if (!digitalRead(KEY_3))   PARKING_MOVMENT(PARKING_UP);
  if (!digitalRead(KEY_4))   PARKING_MOVMENT(PARKING_DOWN);

  if (digitalRead(KEY_1) && digitalRead(KEY_2) && digitalRead(KEY_3) && digitalRead(KEY_4))
  { PARKING_MOVMENT(GATE_STOP);
    PARKING_MOVMENT(PARKING_STOP);
  }
}

void parking_status_display(void)
{ lcd.setCursor(0, 0);
  lcd.print("S1:     S2:     ");
  lcd.setCursor(0, 1);
  lcd.print("S3:     S4:     ");
  if (digitalRead(IR_SLOT_1))
  { lcd.setCursor(3, 0);
    lcd.print("FULL ");
    Slot_1 = 1;
  }
  else
  { lcd.setCursor(3, 0);
    lcd.print("EMPTY");
    Slot_1 = 0;
  }
  if (digitalRead(IR_SLOT_2))
  { lcd.setCursor(11, 0);
    lcd.print("FULL ");
    Slot_2 = 1;
  }
  else
  { lcd.setCursor(11, 0);
    lcd.print("EMPTY");
    Slot_2 = 0;
  }
  if (digitalRead(IR_SLOT_3))
  { lcd.setCursor(3, 1);
    lcd.print("FULL ");
    Slot_3 = 1;
  }
  else
  { lcd.setCursor(3, 1);
    lcd.print("EMPTY");
    Slot_3 = 0;
  }
  if (digitalRead(IR_SLOT_4))
  { lcd.setCursor(11, 1);
    lcd.print("FULL ");
    Slot_4 = 1;
  }
  else
  { lcd.setCursor(11, 1);
    lcd.print("EMPTY");
    Slot_4 = 0;
  }
}

void Passward_Logic(void)
{ if (!digitalRead(IR_GATE_IN))  return;

  lcd.setCursor(0, 0);
  lcd.print("    ENTER OTP   ");

  
  lcd.setCursor(0, 1);
  lcd.print("                ");

  lcd.setCursor(4, 1);
  byte press_count = 0;
  byte EnterPasswardArray[] =  {0, 0, 0, 0, 0};
  while (1)
  { if (!digitalRead(KEY_1))
    { lcd.print('1');
      EnterPasswardArray[press_count] = 1;
      press_count++;
      delay(300);
    }
    if (!digitalRead(KEY_2))
    { lcd.print('2');
      EnterPasswardArray[press_count] = 2;
      press_count++;
      delay(300);
    }
    if (!digitalRead(KEY_3))
    { lcd.print('3');
      EnterPasswardArray[press_count] = 3;
      press_count++;
      delay(300);
    }
    if (!digitalRead(KEY_4))
    { lcd.print('4');
      EnterPasswardArray[press_count] = 4;
      press_count++;
      delay(300);
    }
    if (!digitalRead(KEY_5))
    { lcd.print('5');
      EnterPasswardArray[press_count] = 5;
      press_count++;
      delay(300);
    }
    if (press_count == 5)  break;
  }
  press_count = 0;
  delay(1000);
  for (char i = 0; i < MAX_PASSWARD; i++)
  { for (char j = 0; j < 5; j++)
    { if (EnterPasswardArray[j] == Passward_Array[i][j])
      { press_count++;
      }
    }
    if (press_count == 5)
    { lcd.setCursor(0, 0);
      lcd.print("  OTP MATCHED   ");
      lcd.setCursor(0, 1);
      lcd.print("YOU CAN PARK NOW");
      delay(5000);
      ProccessStep = 1;
      return;
    }
    press_count = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print("OTP NOT MATCHED ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(2000);
  return;
}

void PARKING_MOVMENT(char MOVMENT)
{ if (MOVMENT == PARKING_UP)
  { digitalWrite(PARKING_MOTOR_1, HIGH);
    digitalWrite(PARKING_MOTOR_2, LOW);
  }
  if (MOVMENT == PARKING_DOWN)
  { digitalWrite(PARKING_MOTOR_2, HIGH);
    digitalWrite(PARKING_MOTOR_1, LOW);
  }
  if (MOVMENT == PARKING_STOP)
  { digitalWrite(PARKING_MOTOR_1, LOW);
    digitalWrite(PARKING_MOTOR_2, LOW);
  }
  if (MOVMENT == GATE_OPEN)
  { digitalWrite(GATE_MOTOR_1, HIGH);
    digitalWrite(GATE_MOTOR_2, LOW);
  }
  if (MOVMENT == GATE_CLOSE)
  { digitalWrite(GATE_MOTOR_2, HIGH);
    digitalWrite(GATE_MOTOR_1, LOW);
  }
  if (MOVMENT == GATE_STOP)
  { digitalWrite(GATE_MOTOR_1, LOW);
    digitalWrite(GATE_MOTOR_2, LOW);
  }
}


void Send_Continously(void)
{ ContSending_timer++;
  if (ContSending_timer < 10) return;
  ContSending_timer = 0;

  if (AppContDataSending == 1)
  { AppData_Send(App_LinkID);
  }
}

void GET_NEW_DATA(void)
{
  if (Serial1.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE];
  Data.toCharArray(str_to_char_buff, 50);

  if (compare_string(Data, (char *)"+IPD,", 5))
  { temp =  crnt_match_point;
    if (compare_string(Data, (char *)"APP_", 4))
    { App_LinkID = str_to_char_buff[temp];
      current_motion = 0;

      if (str_to_char_buff[crnt_match_point] == PARKING_UP)
      { PARKING_MOVMENT(PARKING_UP);
        current_motion = 1;
      }
      if (str_to_char_buff[crnt_match_point] == PARKING_DOWN)
      { PARKING_MOVMENT(PARKING_DOWN);
        current_motion = 2;
      }
      if (str_to_char_buff[crnt_match_point] == PARKING_STOP)
      { PARKING_MOVMENT(PARKING_STOP);
        current_motion = 0;
      }
      if (str_to_char_buff[crnt_match_point] == GATE_OPEN)
      { PARKING_MOVMENT(GATE_OPEN);
        current_motion = 3;
      }
      if (str_to_char_buff[crnt_match_point] == GATE_CLOSE)
      { PARKING_MOVMENT(GATE_CLOSE);
        current_motion = 4;
      }
      if (str_to_char_buff[crnt_match_point] == GATE_STOP)
      { PARKING_MOVMENT(GATE_STOP);
        current_motion = 5;
      }

      if (AppContDataSending == 0)
      { AppContDataSending = 1;
        AppData_Send(App_LinkID);
      }
      return;
    }
  }
}

bool AppData_Send(char Link_ID)
{ String data = " ";
  String Send_data = "A" + String(Slot_1) + "B" + String(Slot_2) + "C" + String(Slot_3) + "D" + String(Slot_4) + "_";
  int string_size = Send_data.length();

  Serial1.print("AT+CIPSEND=");
  Serial1.print(Link_ID);
  Serial1.print(",");
  Serial1.println(string_size, DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial1.print(Send_data);
    data = readData(2000);
    if (compare_string(data, (char *)"SEND OK", 7)) return true;
  }
  return false;
}

bool Server_setup(void)
{
  AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if (!AT_CWSAP("Parking_wifi", "1234567890"))  return false;
  if (!AT_CIPSERVER(1234))                       return false;
  return true;
}


bool AT_test(void)
{ String data = "";
  Serial1.println(F("AT"));
  data = readData(500);
  return (compare_string(data, (char *)"OK", 2));
}

void AT_RESTORE(void)
{ Serial1.println(F("AT+RESTORE"));
  readData(500);
}

void AT_RST(void)
{ Serial1.println(F("AT+RST"));
  readData(500);
}

void AT_CWMODE(char Mode)
{ Serial1.print(F("AT+CWMODE="));
  Serial1.println(Mode, DEC);
  readData(500);
}
void AT_CIPMUX(void)
{ String data = "";
  Serial1.println(F("AT+CIPMUX=1"));
  data = readData(500);
}

bool AT_CWSAP(String My_WifiName, String My_Wifipswd)
{ String data = "";
  Serial1.print(F("AT+CWSAP=\""));
  Serial1.print(My_WifiName);
  Serial1.print(F("\",\""));
  Serial1.print(My_Wifipswd);
  Serial1.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}

bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial1.print(F("AT+CIPSERVER=1,"));
  Serial1.println(port_num, DEC);
  data = readData(500);
  return (compare_string(data, (char *)"OK", 2));
}

bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial1.print(F("AT+CIPCLOSE="));
  Serial1.println(linkID);
  data = readData(500);
  return (compare_string(data, (char *)"OK", 2));
}

bool compare_string(String main_str, char *sub_str, char String2_size)
{ int main_str_length = 0, i = 0;
  main_str_length  = main_str.length();
  while (main_str_length != i)
  {
    if (main_str.substring(i, i + String2_size) == sub_str)
    {
      crnt_match_point = i + String2_size;
      return true;
    }
    i++;
  }
  return false;
}

String readData(unsigned long timeout)
{
  String data = "";
  char i = 0;
  unsigned long t = millis();
  while (millis() - t < timeout)
  { if (Serial1.available() > 0)
    { char r = Serial1.read();
      if (i < 50)  data += r;
      i++;
      t = millis();
    }
  }
  Serial.println(data);
  return data;
}
