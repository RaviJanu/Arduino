//!!AUM!!
//Robot work on wifi signal strength 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13);      // RX, TX

#define RIGHT_MOTOR_1     5
#define RIGHT_MOTOR_2     6
#define LEFT_MOTOR_1      7
#define LEFT_MOTOR_2      8
#define RELAY             9
#define OBSTICAL_SENSOR   10


#define ROBO_STOP         'S'
#define ROBO_FORWARD      'F'
#define ROBO_BACKWARD     'B'
#define ROBO_LEFT         'L'
#define ROBO_RIGHT        'R'

int crnt_match_point = 0;
char browser_LinkID = 0,App_LinkID = 0; 
char AppContDataSending = 0;
char connection_flag = 0;

int base_ms = 0;

void setup() 
{ mySerial.begin(9600);             //uart testing
  Serial.begin(115200);            //wifi
  pinMode(RIGHT_MOTOR_1,OUTPUT);
  pinMode(RIGHT_MOTOR_2,OUTPUT);
  pinMode(LEFT_MOTOR_1,OUTPUT);
  pinMode(LEFT_MOTOR_2,OUTPUT);
  pinMode(RELAY,OUTPUT);
  pinMode(OBSTICAL_SENSOR,INPUT);
  RobotMovement(ROBO_STOP);
  digitalWrite(RELAY,LOW);

  delay(3000);
  if(!AT_test())   
  { mySerial.println("WIFI MODULE ERR "); 
  }
  if(!client_setup())   
  { mySerial.println("CLIENT CONN ERR "); 
  }
  
}

#define TURN_90_DELAY       4000 
#define TURN_180_DELAY      TURN_90_DELAY*2     
#define FORWARD_DELAY       2000  
#define STOP_DELAY          1000


int CurrentSignal = 0,PastSignal = 0;
byte StartFlag = 0;

void ObsticalSense(char Direction,unsigned int timer)
{ while(timer > 0)
  {  if(!digitalRead(OBSTICAL_SENSOR))
     {  RobotMovement(ROBO_STOP);
        delay(2000);
     }
     else RobotMovement(Direction);
     delay(1);
//     RobotMovement(ROBO_STOP);
//     delay(2);
     
     timer--; 
  }
}

char Step_Count = 0;

void loop() 
{ GET_NEW_DATA();
  if(StartFlag == 1)
  { FindSignalStrenth();
    
    if(CurrentSignal < 15)       
    { StartFlag = 0;
      mySerial.println("RelayStart");
      digitalWrite(RELAY,HIGH);
      delay(10000);
      digitalWrite(RELAY,LOW);
    }
    else
    { if(CurrentSignal <= PastSignal)  
      { mySerial.println("FORWARD");
        RobotMovement(ROBO_FORWARD);
        ObsticalSense(ROBO_FORWARD,FORWARD_DELAY);
        RobotMovement(ROBO_STOP);
        delay(STOP_DELAY);
      }
      if(CurrentSignal > PastSignal)  
      { mySerial.println("LEFT");
        RobotMovement(ROBO_LEFT);     
        ObsticalSense(ROBO_LEFT,TURN_90_DELAY);
        RobotMovement(ROBO_STOP);
        delay(STOP_DELAY);
        RobotMovement(ROBO_FORWARD);
        ObsticalSense(ROBO_FORWARD,FORWARD_DELAY);
        RobotMovement(ROBO_STOP);
        delay(STOP_DELAY);
      }  
    }
    PastSignal = CurrentSignal;
    
    Step_Count++;
    if(Step_Count > 15)     
    { StartFlag = 0;
      mySerial.println("RelayStart");
      digitalWrite(RELAY,HIGH);
      delay(10000);
      digitalWrite(RELAY,LOW);
    }
  }
  SEND_DATA();
  
/*  if(mySerial.available())
  {   char r = mySerial.read();
      if(r == 'Q')    RobotMovement(ROBO_FORWARD); 
      if(r == 'W')    RobotMovement(ROBO_BACKWARD); 
      if(r == 'E')    RobotMovement(ROBO_LEFT); 
      if(r == 'R')    RobotMovement(ROBO_RIGHT); 
      if(r == 'X')    RobotMovement(ROBO_STOP);    
  }
*/
  delay(1);
}


void RobotMovement(char Direction)
{ if(Direction == ROBO_STOP)
  { digitalWrite(RIGHT_MOTOR_1,LOW);
    digitalWrite(RIGHT_MOTOR_2,LOW);
    digitalWrite(LEFT_MOTOR_1,LOW);
    digitalWrite(LEFT_MOTOR_2,LOW);
  }
   if(Direction == ROBO_FORWARD)
  { digitalWrite(RIGHT_MOTOR_1,HIGH);
    digitalWrite(RIGHT_MOTOR_2,LOW);
    digitalWrite(LEFT_MOTOR_1,HIGH);
    digitalWrite(LEFT_MOTOR_2,LOW);
  }
   if(Direction == ROBO_BACKWARD)
  { digitalWrite(RIGHT_MOTOR_1,LOW);
    digitalWrite(RIGHT_MOTOR_2,HIGH);
    digitalWrite(LEFT_MOTOR_1,LOW);
    digitalWrite(LEFT_MOTOR_2,HIGH);
  }
   if(Direction == ROBO_LEFT)
  { digitalWrite(RIGHT_MOTOR_1,LOW);
    digitalWrite(RIGHT_MOTOR_2,HIGH);
    digitalWrite(LEFT_MOTOR_1,HIGH);
    digitalWrite(LEFT_MOTOR_2,LOW);
  }
   if(Direction == ROBO_RIGHT)
  { digitalWrite(RIGHT_MOTOR_1,HIGH);
    digitalWrite(RIGHT_MOTOR_2,LOW);
    digitalWrite(LEFT_MOTOR_1,LOW);
    digitalWrite(LEFT_MOTOR_2,HIGH);
  }
  
}


void FindSignalStrenth(void)
{ String Data = "";
  char str_to_char_buff[100],temp = 0,i = 0;
  int strenth = 0; 
  Data = AT_CWLAP("MY_WifiStation");
  Data.toCharArray(str_to_char_buff,100);
  compare_string(Data,(char *)",-",2);

  for(i = 0;i<3;i++)
  { if(isdigit(str_to_char_buff[crnt_match_point+i]))
    { temp = str_to_char_buff[crnt_match_point+i] - 0x30;
      strenth = strenth * 10 + temp;
    }
  }
  CurrentSignal =   strenth / 2;
  
  mySerial.println("Signal Strenth "+String(CurrentSignal));
}

void GET_NEW_DATA(void)
{  if (Serial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[50]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      if(compare_string(Data, (char *)"FIR",3))
      { App_LinkID = str_to_char_buff[temp];
        StartFlag = 1;       
        return;
      }
  }
  if(compare_string(Data, (char *)"CLOSED",6))
  { connection_flag = 0;
  }
}


bool client_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWJAP("MY_WifiStation","1234567890"))
  {   mySerial.println("Not Connected to server");
      return false;
  }
  return true;  
}


void SEND_DATA(void)
{ if(connection_flag == 1)  return;
  if(!AT_CIPSTART())
  {  mySerial.println("TCP CONNECT ERROR");
     return;
  }
  if(!AT_CIPSEND())
  {   mySerial.println("DATA SENDING ERROR");
      return;
  }
//  AT_CIPCLOSE();
  connection_flag = 1;
}


bool AT_CIPSTART(void)
{ String data = "";
  bool return_flag = 0;
  Serial.println("AT+CIPSTART=1,\"TCP\",\"192.168.4.1\",80");
  data = readData(5000);
  return_flag = compare_string(data,(char *)"1,CONNECT",9); 
  return(return_flag);
}

bool AT_CIPSEND(void)
{ String Senddata = "APP_";
  int Str_length = Senddata.length();
  String data = "";
  Serial.print("AT+CIPSEND=1,");
  Serial.println(Str_length,DEC);
  data = readData(2000);
  if(compare_string(data,(char *)">",1))
  { Serial.println(Senddata);
    data = readData(8000);
    if(compare_string(data,(char *)"SEND OK",7))
    { return true;
    }
  }
  return false;
}


void AT_CIPCLOSE(void)
{ String data = "";
  Serial.println("AT+CIPCLOSE=1");
  data = readData(1000);
}



String AT_CWLAP(String WifiName)
{ String data = "";
  Serial.println("AT+CWLAP=\""+WifiName+"\"");
  data = readData(3000);
  return(data);
}


bool AT_test(void)
{ String data = "";
  Serial.println(F("AT"));
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ Serial.println(F("AT+RESTORE"));
  readData(2000);
}

void AT_CWMODE(char Mode)
{ Serial.print(F("AT+CWMODE="));
  Serial.println(Mode,DEC);
  readData(500);
}

void AT_CIPMUX(void)
{ String data = "";
  Serial.println(F("AT+CIPMUX=1"));
  data = readData(500);
}


bool AT_CWSAP(String My_WifiName,String My_Wifipswd)
{ String data = "";
  Serial.print(F("AT+CWSAP=\""));
  Serial.print(My_WifiName);
  Serial.print(F("\",\""));
  Serial.print(My_Wifipswd);
  Serial.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}


bool AT_CWJAP(String ipaddr,String passwrd)
{ String data = "";
  Serial.print("AT+CWJAP=\"");
  Serial.print(ipaddr);
  Serial.print("\",\"");
  Serial.print(passwrd);
  Serial.println("\"");
  data = readData(5000);
  return(compare_string(data,(char *)"WIFI CONNECTED",14));
}


bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial.print(F("AT+CIPSERVER=1,"));
  Serial.println(port_num,DEC);
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));    
}

bool compare_string(String main_str,char *sub_str,char String2_size)
{ int main_str_length = 0,i = 0;
  main_str_length  = main_str.length();
  while(main_str_length != i)
  { if(main_str.substring(i,i + String2_size) == sub_str)
    { crnt_match_point = i+String2_size;
      return true; 
    }                                              
    i++;
  }
  return false;
}

String readData(unsigned long timeout) 
{   String data = "";
    char i =0;
    unsigned long t = millis();
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          if(i < 100)  data += r;
          i++;  
          t = millis();
      }
    }
    mySerial.println(data);
    return data;
}
