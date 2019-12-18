//!!AUM!!
#include <avr/pgmspace.h>
#include <Stream.h>

int crnt_match_point = 0;
float TotalConsumtion = 0,UnBilled_Amount = 0;
char PowerFactor = 0;
char Theft_flag = 0;
unsigned char SYS_INPUT[3] = {0,0,0};
char sever_connected_flag = 0;
int base_ms = 0,data_sending_timer = 0;
int CTC_ADC_value = 0;


void setup() 
{ Serial1.begin(115200);
  Serial.begin(115200);
  delay(5000);

  if(!AT_test(500))   
  { Serial.println("Check ESP Module and Restart Arduino");
    while(true);
  }
  if(!client_setup())  
  { sever_connected_flag = 1;
    Serial.println("client not connected");
  } 
}


void loop() 
{GET_NEW_DATA();
 delay(1);
 SEND_DATA();
}


void SEND_DATA(void)
{ if(Serial.available() == 0) return;
  String Data = "";
  Data = readData1(500);
  
  if(!AT_CIPSTART(80))
  {  Serial.println("Not connected to server");
  }
  AT_CIPSEND("GET /receiver.php?apples=1584&oranges=895 HTTP/1.1\r\nHost: jts-demo.000webhostapp.com\r\n");
  AT_CIPCLOSE(500);
}


void GET_NEW_DATA(void)
{ if(Serial1.available() == 0) return; 
  String Data = "";
  char str_to_char_buff[50];
  Data = readData(500);
}




bool client_setup(void)
{ AT_RESTORE(1000);
  AT_RST(500);
  AT_CWMODE(1,500);
  AT_CIPMUX(500);
  
  if(!AT_CWJAP("jeet_2x","ganeshaabc123"))
  {   Serial.println("Router Connection error");
      return false;
  }
  if(!AT_CIPSTART(80))
  {  Serial.println("Not connected to server");
  }
//  AT_CIPSEND("GET /SMSSEND.php?mobilenum=8879230965&message=HELLO_ANIL_NAYAK HTTP/1.1\r\nHost: jts-demo.000webhostapp.com\r\n\r\n");
// AT_CIPSEND("GET /SMSSEND.php HTTP/1.1\r\nHost: jts-demo.000webhostapp.com\r\n\r\n");
  AT_CIPSEND("GET /submitsms.jsp?user=jts123&key=23658c5288XX&mobile=9892790791&message=ESP8266_WIFI_SMS_TEST_BY_RAVI&senderid=JTSPRJ&accusage=1 HTTP/1.1\r\nHost: sms.karvagroup.com\r\n\r\n");
  
  AT_CIPCLOSE(500);
  
  return true;  
}

bool AT_CIPSTART(int port_num)
{ String data = "";
  bool return_flag = 0;
//  Serial1.println("AT+CIPSTART=1,\"TCP\",\"jts-demo.000webhostapp.com\",80");
  Serial1.println("AT+CIPSTART=1,\"TCP\",\"sms.karvagroup.com\",80");
  data = readData(2000);
  return_flag = compare_string(data,(char *)"1,CONNECT",9); 
  return(return_flag);
}

bool AT_CIPSEND(String Send_str)
{ int Str_length = Send_str.length();
  String data = "";
  Serial1.print("AT+CIPSEND=1,");
  Serial1.println(Str_length,DEC);
  data = readData(2000);
  if(compare_string(data,(char *)">",1))
  { Serial1.println(Send_str);
    data = readData(5000);
    if(compare_string(data,(char *)"SEND OK",7))
    { return true;
    }
  }
  return false;
}

void AT_CIPCLOSE(int resp_wait)
{ String data = "";
  Serial1.println("AT+CIPCLOSE=1");
  data = readData(resp_wait);
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
bool AT_CWJAP(String ipaddr,String passwrd)
{ String data = "";
  bool return_flag = 0;
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(ipaddr);
  Serial1.print("\",\"");
  Serial1.print(passwrd);
  Serial1.println("\"");
  data = readData(10000);
  if(compare_string(data,(char *)"WIFI DISCONNECT",15)) return(0);
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14); 
  return(return_flag);
}



bool AT_CWSAP(int resp_wait)
{ String data = "";
  Serial1.println("AT+CWSAP=\"MY_Wifi_User\",\"1234567890\",5,3");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}

bool AT_test(int resp_wait)
{ String data = "";
  Serial1.println("AT");
  data = readData(resp_wait);
  return(compare_string(data,(char *)"OK",2));
}
bool AT_RST(int resp_wait)
{ String data = "";
  Serial1.println("AT+RST");
  data = readData(resp_wait);
}
bool AT_RESTORE(int resp_wait)
{ String data = "";
  Serial1.println("AT+RESTORE");
  data = readData(resp_wait);
}


bool AT_CWMODE(char Mode,int resp_wait)
{ String data = "";
  Serial1.print("AT+CWMODE=");
  Serial1.println(Mode,DEC);
  data = readData(resp_wait);
}

bool AT_CIPMUX(int resp_wait)
{ String data = "";
  Serial1.println("AT+CIPMUX=1");
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
          t = millis();
      }
    }
    Serial.println(data);
    return data;
}


String readData1(unsigned long timeout) 
{   String data = "";
    unsigned long t = millis();
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          data += r;  
          t = millis();
      }
    }
    Serial.println(data);
    return data;
}


