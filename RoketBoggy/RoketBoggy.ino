 //!!AUM!!
//INIT...
//This is IOT based Robotic project where I am controling a robot using mobile through wifi.
//where my robot is a server and mobile is client. 

#define LEFT_MOTOR_1        2
#define LEFT_MOTOR_2        3
#define RIGHT_MOTOR_1       4
#define RIGHT_MOTOR_2       5


#define INPUT_DATA_BUFF_SIZE    50
#define MOV_STOP          0
#define MOV_FORWARD       1
#define MOV_BACKWARD      2
#define MOV_LEFT          3
#define MOV_RIGHT         4

int crnt_match_point = 0;
char App_LinkID = 0;
int base_count = 0;
    
void setup() 
{  // put your setup code here, to run once:
  pinMode(LEFT_MOTOR_1,OUTPUT);
  pinMode(LEFT_MOTOR_2,OUTPUT);
  pinMode(RIGHT_MOTOR_1,OUTPUT);
  pinMode(RIGHT_MOTOR_2,OUTPUT);
  ROBOT_MOVMENT(MOV_STOP);
  
  Serial.begin(115200);
  delay(3000);
    
  if (!AT_test())
  { Serial.println(F("Check ESP Module and Restart Arduino"));
  }
  if (!Server_setup())
  { Serial.println(F("Server Not Setup"));
  }
}

void loop()
{   GET_NEW_DATA();
    delay(1);
}

void ROBOT_MOVMENT(char MOVMENT)
{ if(MOVMENT == MOV_FORWARD)
  {   digitalWrite(LEFT_MOTOR_1,HIGH);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,HIGH);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
  if(MOVMENT == MOV_BACKWARD)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,HIGH);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_LEFT)
  {   digitalWrite(LEFT_MOTOR_1,HIGH);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_RIGHT)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,HIGH);
      digitalWrite(RIGHT_MOTOR_1,HIGH);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
  if(MOVMENT == MOV_STOP)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
}


void GET_NEW_DATA(void)
{ if (Serial.available() == 0) return;
  
  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE];
  Data.toCharArray(str_to_char_buff, 50);

  if (compare_string(Data, (char *)"+IPD,", 5))
  { temp =  crnt_match_point;
    if (compare_string(Data, (char *)"APP_", 4))
    { App_LinkID = str_to_char_buff[temp];
    
      if (str_to_char_buff[crnt_match_point] == 'F') 
      { ROBOT_MOVMENT(MOV_FORWARD);
      }
      if (str_to_char_buff[crnt_match_point] == 'B')  
      { ROBOT_MOVMENT(MOV_BACKWARD);
      }
      if (str_to_char_buff[crnt_match_point] == 'L')  
      { ROBOT_MOVMENT(MOV_LEFT);
      }
      if (str_to_char_buff[crnt_match_point] == 'R') 
      { ROBOT_MOVMENT(MOV_RIGHT);
      }
      if (str_to_char_buff[crnt_match_point] == 'S') 
      { ROBOT_MOVMENT(MOV_STOP);
      }
      
      return;
    }
  }
}


bool Server_setup(void)
{
  AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if (!AT_CWSAP("MY_RobotWifi", "1234567890"))  return false;
  if (!AT_CIPSERVER(1234))                       return false;
  return true;
}


bool AT_test(void)
{ String data = "";
  Serial.println(F("AT"));
  data = readData(500);
  return (compare_string(data, (char *)"OK", 2));
}

void AT_RESTORE(void)
{ Serial.println(F("AT+RESTORE"));
  readData(500);
}

void AT_RST(void)
{ Serial.println(F("AT+RST"));
  readData(500);
}

void AT_CWMODE(char Mode)
{ Serial.print(F("AT+CWMODE="));
  Serial.println(Mode, DEC);
  readData(500);
}
void AT_CIPMUX(void)
{ String data = "";
  Serial.println(F("AT+CIPMUX=1"));
  data = readData(500);
}

bool AT_CWSAP(String My_WifiName, String My_Wifipswd)
{ String data = "";
  Serial.print(F("AT+CWSAP=\""));
  Serial.print(My_WifiName);
  Serial.print(F("\",\""));
  Serial.print(My_Wifipswd);
  Serial.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}

bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial.print(F("AT+CIPSERVER=1,"));
  Serial.println(port_num, DEC);
  data = readData(500);
  return (compare_string(data, (char *)"OK", 2));
}

bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial.print(F("AT+CIPCLOSE="));
  Serial.println(linkID);
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
  { if (Serial.available() > 0)
    { char r = Serial.read();
      if (i < 50)  data += r;
      i++;
      t = millis();
    }
  }
  //    Serial.println(data);
  return data;
}
