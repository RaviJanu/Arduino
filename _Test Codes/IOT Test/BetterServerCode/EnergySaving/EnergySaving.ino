//!AUM!!
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
int crnt_match_point = 0;
char SvrNotSetup_flag = 0;

#define INPUT_DATA_BUFF_SIZE    50
char browser_LinkID = 0,App_LinkID = 0,Module_LinkID = 0; 
char AppContDataSending = 0;

int USER_CTC_value = 1234;

void setup() 
{ mySerial.begin(115200);
  Serial.begin(9600);

  if(!AT_test())   
  { Serial.println(F("Check ESP Module and Restart Arduino"));
    while(true);
  }

  if(Server_setup())  SvrNotSetup_flag = 0;
  else                SvrNotSetup_flag = 1;  
}

void loop() 
{ SvrSetup_Test();  
  GET_NEW_DATA();
  Send_Continously();
  delay(1);
}

int ContSending_timer = 0;
void Send_Continously(void)
{ ContSending_timer++;
  if(ContSending_timer < 10000) return;
  ContSending_timer = 0;

  if(AppContDataSending == 1)
  {   AppData_Send(App_LinkID);
  }
}


void GET_NEW_DATA(void)
{ if (mySerial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      Serial.println(temp);
      if(compare_string(Data, (char *)":GET ",5))
      { browser_LinkID = str_to_char_buff[temp];
        HTML_UPLOAD(browser_LinkID);  
        return;
      }
      if(compare_string(Data, (char *)"APP_",4))
      { App_LinkID = str_to_char_buff[temp];
        AppData_Send(App_LinkID);
        AppContDataSending = 1;       
        return;
      }
  }
  
}


bool AppData_Send(char Link_ID)
{ String data = "";
  mySerial.print("AT+CIPSEND=");
  mySerial.print(Link_ID);
  mySerial.print(",");
  mySerial.println(10,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { mySerial.print("A");
    mySerial.print(USER_CTC_value);
    mySerial.println("               ");
    data = readData(1000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
}

const PROGMEM String html_page1 = "<html><body><center><h1>Electrical energy saving using IOT<br>S.P.I.T. College</h1></center></html></body>";
bool HTML_UPLOAD(char Link_ID)
{ String data = "";

  int Str_length = 0;
  Str_length +=  html_page1.length();

  mySerial.print("AT+CIPSEND=");
  mySerial.print(Link_ID);
  mySerial.print(",");
  mySerial.println(Str_length,DEC);
  data = readData(500);
  if (compare_string(data, (char *)">", 1))
  { mySerial.println(html_page1);
    data = readData(1000);
    if(compare_string(data,(char *)"SEND OK", 7))
    { AT_CIPCLOSE(Link_ID);
      return true;
    }
  }
  AT_CIPCLOSE(Link_ID);
  return false;
}



void SvrSetup_Test(void)
{ if(SvrNotSetup_flag == 0) return;
  if(Server_setup())        SvrNotSetup_flag = 0;
  else                      SvrNotSetup_flag = 1; 
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
  mySerial.println(F("AT"));
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ mySerial.println(F("AT+RESTORE"));
  readData(500);
}

void AT_RST(void)
{ mySerial.println(F("AT+RST"));
  readData(500);
}

void AT_CWMODE(char Mode)
{ mySerial.print(F("AT+CWMODE="));
  mySerial.println(Mode,DEC);
  readData(500);
}
void AT_CIPMUX(void)
{ String data = "";
  mySerial.println(F("AT+CIPMUX=1"));
  data = readData(500);
}


bool AT_CWSAP(String My_WifiName,String My_Wifipswd)
{ String data = "";
  mySerial.print(F("AT+CWSAP=\""));
  mySerial.print(My_WifiName);
  mySerial.print(F("\",\""));
  mySerial.print(My_Wifipswd);
  mySerial.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}


void AT_CIFSR(void)
{ mySerial.println(F("AT+CIFSR"));
  readData(500);
}


bool AT_CIPSERVER(int port_num)
{ String data = "";
  mySerial.print(F("AT+CIPSERVER=1,"));
  mySerial.println(port_num,DEC);
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));    
}

bool AT_CIPSEND(char linkID,String Send_str)
{ int Str_length = 0;
  String data = "";

  Str_length = Send_str.length();
  mySerial.print("AT+CIPSEND=");
  mySerial.write(linkID);
  mySerial.write(',');
  mySerial.println(Str_length);
  
  data = readData(500);
  
  if (compare_string(data, (char *)">", 1))
  { mySerial.println(Send_str);
    data = readData(1000);
    if (compare_string(data, (char *)"SEND OK", 7))
    { return true;
    }
  }
  return false;
}

bool AT_CIPCLOSE(char linkID)
{ String data = "";
  mySerial.print(F("AT+CIPCLOSE="));
  mySerial.println(linkID);
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
    { if(mySerial.available() > 0) 
      {   char r = mySerial.read();
          data += r;  
          t = millis();
      }
    }
    Serial.println(data);
    return data;
}










