//!!AUM!!
#include <avr/pgmspace.h>
#define INPUT_DATA_BUFF_SIZE    50

#define SWITCH   8


int crnt_match_point = 0;
int send_pulse_value = 0;
char browser_LinkID = 0,App_LinkID = 0 ; 
char AppContDataSending = 0;
int ContSending_timer = 0;
char SWITCH_FLAG = 0;


void setup() 
{  // put your setup code here, to run once:
  Serial2.begin(9600);      //gps
  Serial1.begin(115200);      //wifi
  Serial.begin(115200);     //uart 
  pinMode(SWITCH,INPUT_PULLUP);

  if(!AT_test())   
  {
    Serial.print("WifiModule Error");
    delay(5000);
  }

  if(!Server_setup())  
  { 
    Serial.print("WifiModule Error");
    delay(5000);
  }


}

char health_prob_flag = 0;
int base_count = 0;
void loop() 
{ // put your main code here, to run repeatedly:
  GET_NEW_DATA();
  if(digitalRead(SWITCH))  
  {   if(SWITCH_FLAG == 0)  SWITCH_FLAG = 1;
      else                  SWITCH_FLAG = 0;
      delay(500);
  }
}



void GET_NEW_DATA(void)
{ if (Serial1.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      if(compare_string(Data, (char *)":GET ",5))
      { browser_LinkID = str_to_char_buff[temp];
        HTML_UPLOAD(browser_LinkID);  
        return;
      }
  }
}





bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWSAP("MY_WifiServer","1234567890"))  return false;
  if (!AT_CIPSERVER(1234))                     return false;
  return true;
}


bool AT_test(void)
{ String data = "";
  Serial1.println(F("AT"));
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));
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
  Serial1.println(Mode,DEC);
  readData(500);
}
void AT_CIPMUX(void)
{ String data = "";
  Serial1.println(F("AT+CIPMUX=1"));
  data = readData(500);
}

bool AT_CWSAP(String My_WifiName,String My_Wifipswd)
{ String data = "";
  Serial1.print(F("AT+CWSAP=\""));
  Serial1.print(My_WifiName);
  Serial1.print(F("\",\""));
  Serial1.print(My_Wifipswd);
  Serial1.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}


void AT_CIFSR(void)
{ Serial1.println(F("AT+CIFSR"));
  readData(500);
}


bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial1.print(F("AT+CIPSERVER=1,"));
  Serial1.println(port_num,DEC);
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));    
}

bool AT_CIPSEND(char linkID,String Send_str)
{ int Str_length = 0;
  String data = "";

  Str_length = Send_str.length();
  Serial1.print("AT+CIPSEND=");
  Serial1.write(linkID);
  Serial1.write(',');
  Serial1.println(Str_length);
  
  data = readData(500);
  
  if (compare_string(data, (char *)">", 1))
  { Serial1.println(Send_str);
    data = readData(2000);
    if (compare_string(data, (char *)"SEND OK", 7))
    { return true;
    }
  }
  return false;
}




const String html_page1 = "<html><body><head><meta http-equiv=\"refresh\" content=\"20\"></head><center><h1>IOT TESTING<br> SWITCH : ";   
const String html_page3 = "</h3></center></html></body>";

bool HTML_UPLOAD(char Link_ID)
{ String data = "";

  int Str_length = 0;
  Str_length +=  html_page1.length();
  Str_length +=  html_page3.length();
  Str_length += 3;

  Serial1.print("AT+CIPSEND=");
  Serial1.print(Link_ID);
  Serial1.print(",");
  Serial1.println(Str_length,DEC);
  data = readData(500);
  if (compare_string(data, (char *)">", 1))
  { Serial1.print(html_page1);
    if(SWITCH_FLAG == 0)   Serial1.print("ON ");
    else                   Serial1.print("OFF");
    Serial1.println(html_page3);
    Serial1.print("                   ");

    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7))
    { AT_CIPCLOSE(Link_ID);
      return true;
    }
  }
  AT_CIPCLOSE(Link_ID);
  return false;
}



bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial1.print(F("AT+CIPCLOSE="));
  Serial1.println(linkID);
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
    while(millis() - t < timeout) 
    { if(Serial1.available() > 0) 
      {   char r = Serial1.read();
          data += r;  
          t = millis();
      }
    }
    Serial.println(data);
    return data;
}



