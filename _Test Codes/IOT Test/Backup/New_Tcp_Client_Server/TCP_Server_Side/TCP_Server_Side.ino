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
  if(!Server_setup())  
  { while(true)
    {  AT_RESTORE(1000);
       if(Server_setup())  break;
       delay(10000);    
    }
  } 
}

void NewData_for_Server(void)
{ if(Serial.available() == 0) return;

   String Data = "";
    Data = readData(500); 
    if(!compare_string(Data,(char *)"ERROR",5))
    {     if(compare_string(Data,(char *)"HTTP",4))
          { if(compare_string(Data,(char *)"CLOSED",6))
            { Serial.println(F("Connection Closed : "));    
            } 
            else
            { 
              AT_CIPSEND("<h1>HELLO WORLD!</h1>");
            }
          }
          else  Serial.println(Data); 
    } 
    
}

void loop() 
{ NewData_for_Server();
}


bool Server_setup(void)
{ AT_RST(500);
  AT_CWMODE(3,500);
  AT_CIPMUX(1,500);
  if(!AT_CIPSERVER(1234,1000))
  {   Serial.println("Server Not Init");
      return false;
  }
  delay(5000);
  AT_CIFSR(1000);  
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

bool AT_CIPSERVER(int port_num,int resp_wait)
{ String data = "";
  Serial.print("AT+CIPSERVER=1,");
  Serial.println(port_num,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));    
}
void AT_CIFSR(int resp_wait)
{ String data = "";
  Serial.println("AT+CIFSR");
  data = readData(resp_wait);  
  Serial.println(data);
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
  Serial.print("AT+CWJAP=\"");
  Serial.print(ipaddr);
  Serial.print("\",\"");
  Serial.print(passwrd);
  Serial.println("\"");
  data = readData(10000);
  return(compare_string(data,(char *)"WIFI CONNECTED",14));
}

bool AT_CIPSTART(char link_num,String ipaddr,int port_num)
{ String data = "";
  Serial.print("AT+CIPSTART=");
  Serial.print(link_num,DEC);
  Serial.print(",\"TCP\",\""); 
  Serial.print(ipaddr);
  Serial.print("\",");
  Serial.println(port_num,DEC);  
  data = readData(10000);
  return(compare_string(data,(char *)"CONNECT",7));
}


bool AT_CIPSEND(String Send_str)
{ int Str_length = Send_str.length();
  String data = "";

  Serial.print("AT+CIPSEND=1,");
  Serial.println(Str_length,DEC);
  data = readData(500);
  if(compare_string(data,(char *)">",1))
  { Serial.println(Send_str);
    data = readData(1000);
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





