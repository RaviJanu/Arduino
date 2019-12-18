//!AUM!!
//IOT , Ultrasonic , RFID, 
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX

#define DP_SW1    8
#define DP_SW2    9
#define DP_SW3    10
#define DP_SW4    11

#define US_1_Trig     2
#define US_1_Echo     3
#define US_2_Trig     4
#define US_2_Echo     5
#define LPG_SENSOR    6


#define INPUT_DATA_BUFF_SIZE    50
int crnt_match_point = 0,ContSending_timer = 0;
char browser_LinkID = 0,App_LinkID = 0,Module_LinkID = 0; 
char AppContDataSending = 0;

byte Dustbin1_Size = 0,Dustbin2_Size = 0;
byte area_num = 0,LpgFlag = 0;

void setup() 
{ mySerial.begin(9600);
  Serial.begin(115200);  
  pinMode(DP_SW1, INPUT_PULLUP);
  pinMode(DP_SW2, INPUT_PULLUP);
  pinMode(DP_SW3, INPUT_PULLUP);
  pinMode(DP_SW4, INPUT_PULLUP);
  pinMode(US_1_Echo, INPUT);
  pinMode(US_2_Echo, INPUT);
  pinMode(LPG_SENSOR,INPUT);
  pinMode(US_1_Trig, OUTPUT);
  pinMode(US_2_Trig, OUTPUT);
  digitalWrite(US_1_Trig, LOW);
  digitalWrite(US_2_Trig, LOW);
  delay(3000);

  if(!AT_test())   
  { Serial.println(F("Check ESP Module and Restart Arduino"));
  }
  if(!Server_setup()) Serial.println(F("Server Not Setup"));  
}

void loop() 
{// put your main code here, to run repeatedly:
  DPswitchSence();
  UltraSonicSensce();
  if(!digitalRead(LPG_SENSOR))        LpgFlag = 1;
  else                                LpgFlag = 0;
  Send_Continously();
  GET_NEW_DATA();
  delay(1000);
  /*
  Serial.print("D: ");
  Serial.print(Dustbin1_Size);   
  Serial.print(" W: ");
  Serial.print(Dustbin2_Size);
  Serial.print(" L: ");
  Serial.print(area_num);
  Serial.print(" G: ");
  Serial.println(LpgFlag);
*/
}


void  UltraSonicSensce(void)
{   int   duration_1  = 0,duration_2  = 0,duration_3  = 0; 

    digitalWrite(US_1_Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_1_Trig, LOW);
    duration_1 = microsecondsToInches(pulseIn(US_1_Echo, HIGH));  
    if(duration_1 < 10)
    { Dustbin1_Size = (10 - duration_1)*10;
    }
    else  Dustbin1_Size = 0;
    
    digitalWrite(US_2_Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_2_Trig, LOW);
    duration_2 = microsecondsToInches(pulseIn(US_2_Echo, HIGH));  

    if(duration_2 < 10)
    { Dustbin2_Size = (10 - duration_2)*10;
    }
    else  Dustbin2_Size = 0;

}


long microsecondsToInches(long microseconds)
{ // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}


void DPswitchSence(void)
{ char DPSwitch1 = 0,DPSwitch2 = 0,DPSwitch3 = 0,DPSwitch4 = 0,DP_Data = 0;
  DPSwitch1 = digitalRead(DP_SW1);
  DPSwitch2 = digitalRead(DP_SW2);
  DPSwitch3 = digitalRead(DP_SW3);
  DPSwitch4 = digitalRead(DP_SW4);
  if(DPSwitch1 == 1)  DP_Data |= 0x01;
  if(DPSwitch2 == 1)  DP_Data |= 0x02;
  if(DPSwitch3 == 1)  DP_Data |= 0x04;
  if(DPSwitch4 == 1)  DP_Data |= 0x08;

  switch(DP_Data)
  { case 0x01:  area_num = 1;
    break;
    case 0x02:  area_num = 2;
    break;
    case 0x04:  area_num = 3;
    break;
    case 0x08:  area_num = 4;
    break;
    default :   area_num = 0;
    break;
  }
}


void Send_Continously(void)
{ ContSending_timer++;
  if(ContSending_timer < 10) return;
  ContSending_timer = 0;

  if(AppContDataSending == 1)
  {   AppData_Send(App_LinkID);
  }
}



void GET_NEW_DATA(void)
{ if (Serial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      if(compare_string(Data, (char *)"PP_",3))
      { App_LinkID = str_to_char_buff[temp];
        AppData_Send(App_LinkID);
        AppContDataSending = 1;       
        return;
      }
  }
  if(compare_string(Data, (char *)"CLOSED",6))
  { if(str_to_char_buff[crnt_match_point-8] == App_LinkID)  AppContDataSending = 0;
  }
}


bool AppData_Send(char Link_ID)
{ String data = "";     //"A000B000C0D0_"
  String SendData = "A000B000C0D0_";
  int string_size = SendData.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.write('A');
    if(Dustbin1_Size < 100)   Serial.write('0');
    if(Dustbin1_Size < 10)    Serial.write('0');
    Serial.print(Dustbin1_Size);
    Serial.write('B');
    if(Dustbin2_Size < 100)   Serial.write('0');
    if(Dustbin2_Size < 10)    Serial.write('0');
    Serial.print(Dustbin2_Size);
    Serial.write('C');
    Serial.print(area_num);
    Serial.write('D');
    Serial.print(LpgFlag);
    Serial.write('_');
    
    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
}


bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWJAP("hotspot","1234567890"))
  {   Serial.println("Not Connected to server");
      return false;
  }
//  if(!AT_CWSAP("DUST","1234567890"))  return false;
  if (!AT_CIPSERVER(1234))                     return false;
  return true;
}


bool AT_CWJAP(String ipaddr,String passwrd)
{ String data = "";
  bool return_flag = 0;
  Serial.print("AT+CWJAP=\"");
  Serial.print(ipaddr);
  Serial.print("\",\"");
  Serial.print(passwrd);
  Serial.println("\"");
  data = readData(10000);
  if(compare_string(data,(char *)"WIFI DISCONNECT",15)) return(0);
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14); 
  return(return_flag);
}

bool AT_test(void)
{ String data = "";
  Serial.println(F("AT"));
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ Serial.println(F("AT+RESTORE"));
  readData(500);
}

void AT_UART(long Uart_value)
{ Serial.println("AT+UART="+String(Uart_value)+",8,1,0,0");
  readData(2000);
}


void AT_RST(void)
{ Serial.println(F("AT+RST"));
  readData(500);
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


bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial.print(F("AT+CIPSERVER=1,"));
  Serial.println(port_num,DEC);
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));    
}


bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial.print(F("AT+CIPCLOSE="));
  Serial.println(linkID);
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
    unsigned long t = millis();
    int i = 0;
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          if(i < 50)  data += r;  
          t = millis();
          i++;
      }
    }
//    Serial.println(data);
    return data;
}
