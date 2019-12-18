//!!AUM!!
#include <Stream.h>
int crnt_match_point = 0;

void setup() 
{ Serial.begin(115200);
  Serial1.begin(115200);
  delay(3000);

  if(!AT_test(500))   
  { Serial.println("Check ESP Module and Restart Arduino");
    while(true);
  }
  AT_RESTORE(1000);
  if(!Server_setup())  
  { while(true)
    {  AT_RESTORE(1000);
       if(Server_setup())  break;
       delay(10000);    
    }
  } 
}

void NewData_for_Server(void)
{   if(Serial1.available() == 0) return;

    String Data = "";
    char str_to_char_buff[50];
    Data = readData(500); 

    if(compare_string(Data,(char *)"+IPD",4))
    { Data = Data.substring(crnt_match_point+16);
      Data.toCharArray(str_to_char_buff,40);
      Serial.println(str_to_char_buff); 
      
      if(compare_string(str_to_char_buff,(char *)"Upload",6))
      {  compare_string(str_to_char_buff,(char *)"Message",7);
         Serial.println(&str_to_char_buff[crnt_match_point + 8]); 
      }
      if(compare_string(str_to_char_buff,(char *)"SYS",3))
      {  Serial.println(&str_to_char_buff[crnt_match_point+3]); 
      } 
      if(compare_string(Data,(char *)"CLOSED",6))
      { Serial.println("Connection Closed");    
        return;
      } 
//      AT_CIPSEND("<html><body><form action=\"Upload\">Send Message<input type=\"text\" name=\"Message\"><input type=\"submit\"></form><br>LED <button onClick='location.href=\"SYSL11\"'>ON</button><button onClick='location.href=\"SYSL10\"'>OFF</button></html></body>");
        AT_CIPSEND("<html><body>LED <button onClick='location.href=\"_1\"'>01</button></html></body>");
      delay(5000);
    }
}

void loop() 
{ NewData_for_Server();
}


bool Server_setup(void)
{ AT_RST(500);
  AT_CWMODE(3,500);
  AT_CIPMUX(1,500);
  if(!AT_CIPSERVER(80,1000))
  {   Serial.println("Server Not Init");
      return false;
  }
  AT_CIFSR(1000);  
  return true;
}

bool AT_test(int resp_wait)
{ String data = "";
  Serial1.println("AT");
  data = readData(resp_wait);
  Serial1.println("AT");
  return(compare_string(data,(char *)"OK",2));
}
bool AT_RST(int resp_wait)
{ String data = "";
  Serial1.println("AT+RST");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}
bool AT_RESTORE(int resp_wait)
{ String data = "";
  Serial1.println("AT+RESTORE");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}

bool AT_CIPSERVER(int port_num,int resp_wait)
{ String data = "";
  Serial1.print("AT+CIPSERVER=1,");
  Serial1.println(port_num,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));    
}
bool AT_CIFSR(int resp_wait)
{ String data = "";
  Serial1.println("AT+CIFSR");
  data = readData(resp_wait);  
  return(compare_string(data,(char *)"OK",2));  
}

bool AT_CWMODE(char Mode,int resp_wait)
{ String data = "";
  Serial1.print("AT+CWMODE=");
  Serial1.println(Mode,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}

bool AT_CIPMUX(char Mode,int resp_wait)
{ String data = "";
  Serial1.print("AT+CIPMUX=");
  Serial1.println(Mode,DEC);
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}

bool AT_CWJAP(String ipaddr,String passwrd)
{ String data = "";
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(ipaddr);
  Serial1.print("\",\"");
  Serial1.print(passwrd);
  Serial1.println("\"");
  data = readData(10000);
  return(compare_string(data,(char *)"WIFI CONNECTED",14));
}

bool AT_CIPSTART(char link_num,String ipaddr,int port_num)
{ String data = "";
  Serial1.print("AT+CIPSTART=");
  Serial1.print(link_num,DEC);
  Serial1.print(",\"TCP\",\""); 
  Serial1.print(ipaddr);
  Serial1.print("\",");
  Serial1.println(port_num,DEC);  
  data = readData(10000);
  return(compare_string(data,(char *)"CONNECT",7));
}


bool AT_CIPSEND(String Send_str)
{ int Str_length = Send_str.length();
  String data = "";

  Serial1.print("AT+CIPSEND=0,");
  Serial1.println(Str_length,DEC);
  data = readData(500);
  if(compare_string(data,(char *)">",1))
  { Serial1.println(Send_str);
    data = readData(1000);
    if(compare_string(data,(char *)"SEND OK",7))
    { AT_CIPCLOSE(500);
      return true;
    }
  }
  AT_CIPCLOSE(500);
  return false;
}

void AT_CIPCLOSE(int resp_wait)
{ String data = "";
  Serial1.println("AT+CIPCLOSE=0");
  data = readData(resp_wait);
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
    { if(Serial1.available() > 0) 
      {   char r = Serial1.read();
          data += r;  
      }
    }
    Serial.println(data);
    return data;
}





