//!!AUM!!
#include <Stream.h>
int crnt_match_point = 0;

void setup() 
{ Serial.begin(115200);
  delay(3000);

  if(!AT_test(500))   
  { Serial.println("Check ESP Module and Restart Arduino");
    while(true);
  }
  
  if(!client_setup())  
  { while(true)
    { AT_RESTORE(1000);
      if(client_setup())  break;
      delay(10000);
    }
  } 
}

const char send_data[10][3] = {"D1","D2","D3","D4","D5","D6","D7","D8","D9","D0"};
char count = 0;
int base_ms = 0;
void loop() 
{
 SEND_DATA(); 
 Connection_Check();
 delay(1);
}
void Connection_Check(void)
{ if(Serial.available() == 0) return; 

  String Data = "";
  Data = readData(500); 
  if(compare_string(Data,(char *)"WIFI DISCONNECT",15))
  { while(true)
    { AT_RESTORE(1000);
      if(client_setup())  break;
      delay(10000);
    }
  } 
  
}

void SEND_DATA(void)
{ base_ms++;
  if(base_ms < 5000) return;
  base_ms = 0;

  if(AT_CIPSEND(send_data[count]))
  {  count++;
    if(count > 9) count = 0;
  } 
}

bool client_setup(void)
{ AT_RST(500);
  AT_CWMODE(1,500);
  AT_CIPMUX(1,500);
  if(!AT_CWJAP("AI-THINKER_1049B6",""))
  {   Serial.println("Not Connected to server");
      return false;
  }
  delay(5000);
  if(!AT_CIPSTART(1,"192.168.4.1",1234))
  {  Serial.println("TCP protocal not Init with server");
     return false;
  }
  return true;  
}


bool AT_test(int resp_wait)
{ String data = "";
  Serial.println("AT");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}
bool AT_RST(int resp_wait)
{ String data = "";
  Serial.println("AT+RST");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}
bool AT_RESTORE(int resp_wait)
{ String data = "";
  Serial.println("AT+RESTORE");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}


bool AT_CWMODE(char Mode,int resp_wait)
{ String data = "";
  Serial.print("AT+CWMODE=");
  Serial.println(Mode,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}

bool AT_CIPMUX(char Mode,int resp_wait)
{ String data = "";
  Serial.print("AT+CIPMUX=");
  Serial.println(Mode,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
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
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14);
//  Serial.println(data);   
  return(return_flag);
}

bool AT_CIPSTART(char link_num,String ipaddr,int port_num)
{ String data = "";
  bool return_flag = 0;
  Serial.print("AT+CIPSTART=");
  Serial.print(link_num,DEC);
  Serial.print(",\"TCP\",\""); 
  Serial.print(ipaddr);
  Serial.print("\",");
  Serial.println(port_num,DEC);  
  data = readData(10000);
  return_flag = compare_string(data,(char *)"1,CONNECT",9);
//  Serial.println(data);   
  return(return_flag);
}


bool AT_CIPSEND(String Send_str)
{ int Str_length = Send_str.length();
  String data = "";

  Serial.print("AT+CIPSEND=1,");
  Serial.println(Str_length,DEC);
  data = readData(2000);
  if(compare_string(data,(char *)">",1))
  { Serial.println(Send_str);
    data = readData(10000);
    if(compare_string(data,(char *)"SEND OK",7))
    { return true;
    }
  }
  
  return false;
}


bool compare_string(String main_str,char *sub_str,char String2_size)
{ int main_str_length = 0,i = 0;
  main_str_length  = main_str.length();

  while(main_str_length != i)
  { if(main_str.substring(i,i + String2_size) == sub_str)
    { crnt_match_point = i;
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
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          data += r;  
      }
    }
    return data;
}




