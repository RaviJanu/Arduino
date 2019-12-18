#include <Stream.h>

String GlobalString;
int crnt_match_point = 0;
char wifi_disconnect_flag = 0;
char Closed_conn_count = 0;

void setup() 
{// put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200);  
  Serial.println("ESP_Module Testing...");
  delay(3000);
  
  if(AT_test(500))    Serial.println("ESP_Module Testing OK");
  else
  { Serial.println("Check ESP Module and Restart Arduino");
    while(true);
  }

  AT_RST(1000);
//AT_ECHO(1000);
  AT_CWMODE(3,500);
  AT_CIPMUX(1,500);
  AT_CIPSERVER(80,500);
  AT_CIFSR(2000);

}

void loop() 
{  // put your main code here, to run repeatedly:
  NewData_for_Server();
  Module_Setting_restore();
  if(Serial.available() > 0) 
  {   if(Serial.read() == 'R')   
      {   wifi_disconnect_flag = 1;
       }
  }
  delay(20);
}

void NewData_for_Server(void)
{   if(Serial1.available() == 0) return;

    String data = "";
    char str_to_char_buff[50];
    data = readData(1000);  
    Serial.println(data);   
    
    if(compare_string(data,(char *)"+IPD",4))
    { data.toCharArray(str_to_char_buff,40);
      Serial.println(&str_to_char_buff[crnt_match_point+15]);   

      if(compare_string(data,(char *)"CLOSED",6))
      { Serial.print("Connection Closed : ");    
        Serial.println(Closed_conn_count,DEC);    
        Closed_conn_count++;
        if(Closed_conn_count > 5)   
        { Closed_conn_count = 0;
          wifi_disconnect_flag = 1;
        } 
        return;
      } 
      else
      { Send_Data(&str_to_char_buff[crnt_match_point+15]);
      }
    }
}

String MAIN_FRAME = "<html><button onClick='location.href=\"F1\"'>F1</button><button id=\"F2\" onClick='location.href=\"F2\"'>F2</button></html>";
String SEC_FRAME = "<h1>HELLO WORLD!</h1>";

void Send_Data(char *action_data)
{  
  if(compare_string(action_data,(char *)"F1",2))
  { AT_CIPSEND(SEC_FRAME,1000);  
    return;
  }
  else
  { AT_CIPSEND(MAIN_FRAME,1000); 
    return;
  }
}

bool AT_CIPSEND(String Send_str,int resp_wait)
{ int Str_length = Send_str.length();
  String data = "";
  bool return_flag;

  Serial1.print("AT+CIPSEND=0,");
  Serial1.println(Str_length,DEC);
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)">",1))
  { Serial1.println(Send_str);
    data = readData(1000);
    Serial.println(data);
    if(compare_string(data,(char *)"SEND OK",7))
    { Serial.println("Data Sent."); 
      return_flag = true;
    }
    else
    { Serial.println("Data Sending Error...");
      return_flag = false;
    }
  }
  else
  { Serial.println("> Not Found");
    return_flag = false;
  }

  AT_CIPCLOSE(500);
  return return_flag;
}


void Module_Setting_restore(void)
{ if(wifi_disconnect_flag == 0) return;
  wifi_disconnect_flag = 0;

  Serial.println("Need Module Retore Command, Press : R");
  while(1)
  {  if(Serial.available() > 0) 
      {   if(Serial.read() == 'R')   break;
          else   Serial.println("Press R");
      }
  }
  
  AT_RESTORE(1000);
  AT_RST(500);
  AT_CWMODE(3,500);
  AT_CIPMUX(1,500);
  AT_CIPSERVER(80,500);
  AT_CIFSR(1000);  
}



bool AT_test(int resp_wait)
{ String data = "";
  Serial1.println("AT");
  data = readData(resp_wait);
  Serial.println(data);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RST(int resp_wait)
{ String data = "";
  Serial1.println("AT+RST");
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    Serial.println("AT+RST OK");
  else                                       Serial.println("AT+RST Fail");
}


void AT_RESTORE(int resp_wait)
{ String data = "";
  Serial1.println("AT+RESTORE");
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    Serial.println("Module Restore OK");
  else                                       Serial.println("Module Restore Fail");
}

void AT_ECHO(int resp_wait)
{ String data = "";
  Serial1.println("ATE0");
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    Serial.println("ECHO OFF");
  else                                       Serial.println("ECHO ONN");
}
void AT_CWMODE(char Mode,int resp_wait)
{ String data = "";
  Serial1.print("AT+CWMODE=");
  Serial1.println(Mode,DEC);
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    
  { Serial.print("MODE Changed to ");
    Serial.println(Mode,DEC);
  }
  else                                       Serial.println("Error In CWMODE");
}

void AT_CIPMUX(char Mode,int resp_wait)
{ String data = "";
  Serial1.print("AT+CIPMUX=");
  Serial1.println(Mode,DEC);
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    
  { Serial.print("Multi Connection set to ");
    Serial.println(Mode,DEC);
  }
  else 
    Serial.println("Multi Connection set Fail");
}

void AT_CIPSERVER(char port_num,int resp_wait)
{ String data = "";
  Serial1.print("AT+CIPSERVER=1,");
  Serial1.println(port_num,DEC);
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"OK",2))    
  { Serial.print("Server Init, Active On port ");
    Serial.println(port_num,DEC);
  }
  else 
    Serial.println("Server Init Fail");
}

void AT_CIFSR(int resp_wait)
{ String data = "";
  char str_to_char_buff[40];
  
  Serial1.println("AT+CIFSR");
  data = readData(resp_wait);
  Serial.println(data);  
  if(compare_string(data,(char *)"APIP",4))    Serial.print("Server IP Address : ");
  else                                          
  { Serial.println("Server Not Created : ");
    return false;
  }
  data.toCharArray(str_to_char_buff,36);
  Serial.println(&str_to_char_buff[crnt_match_point+6]);    
}



void AT_CIPCLOSE(int resp_wait)
{ String data = "";
  Serial1.println("AT+CIPCLOSE=0");
  data = readData(resp_wait);
  Serial.println(data);
  if(compare_string(data,(char *)"CLOSED",6))     Serial.println("Connection Closed");
  else                                            Serial.println("Connection Closing Error");
}

void check_Wifi_connection(String main_str)
{ int main_str_length = 0,i = 0;
  main_str_length  = main_str.length();
  while(main_str_length != i)
  { if(main_str.substring(i,i + 15) == "WIFI DISCONNECT") wifi_disconnect_flag = 1;       
    else    wifi_disconnect_flag = 0;
    i++;
  }  
}



bool compare_string(String main_str,char *sub_str,char String2_size)
{ int main_str_length = 0,i = 0;
  main_str_length  = main_str.length();

  check_Wifi_connection(main_str);
  
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
    return data;
}



