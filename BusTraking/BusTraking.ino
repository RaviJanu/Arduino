//!!AUM!!
// GPS, IOT, 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13);      // RX, TX
#include <TinyGPS.h>
TinyGPS gps; // create gps object


long lat = 0,lon = 0;
long latitude = 0,longitude= 0;

int   crnt_match_point = 0;
char  browser_LinkID = 0,App_LinkID = 0; 
char  AppContDataSending = 0;
int   ContSending_timer = 0;
char  wifi_error = 0;

unsigned int base_ms = 0;


void setup() 
{ Serial.begin(115200);       //WIFI MODULE
  mySerial.begin(9600);       //GPS MODULE

  delay(3000);
  if(!AT_test())   
  { Serial.println(F("Check ESP Module and Restart Arduino"));
    wifi_error = 1;
  }
  if(!Server_setup())
  { Serial.println(F("Server Not Setup")); 
    wifi_error = 1;
  }
}


void loop()
{   GET_NEW_DATA(); 
    
    delay(10);
    base_ms++;
    if(base_ms < 100)  return;
    base_ms = 0;
  
    gps_read();
    Send_Continously();    
    if(wifi_error == 1) 
    { Serial.println("lat:"+String(lat) + " lon:"+String(lon));
    }
}



void gps_read(void)
{ while(mySerial.available())
  {// check for gps data
    char gps_read = mySerial.read();
    if(wifi_error == 1)   Serial.print(gps_read);
    if(gps.encode(gps_read))
    { // encode gps data
      gps.get_position(&lat,&lon); // get latitude and longitude
      // display position
      if(lat != 0)   latitude = lat;
      if(lon != 0)   longitude = lon; 
    }
  }   
}



//----------Wifi Code-------------------------------------
void GET_NEW_DATA(void)
{ if (Serial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[50]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      if(compare_string(Data, (char *)"APP_",4))
      { App_LinkID = str_to_char_buff[temp];
        AppContDataSending = 1;       
        AppData_Send(App_LinkID);
        return;
      }
  }
  if(compare_string(Data, (char *)"CLOSED",6))
  { if(str_to_char_buff[crnt_match_point-8] == App_LinkID)  AppContDataSending = 0;
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



bool AppData_Send(char Link_ID)
{ String data = "A00000000B00000000_";
  int string_size = data.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.write("A");       
    if(latitude == 0)   Serial.print("0000000"); 
    Serial.print(latitude);  
    Serial.write("B");   
    if(longitude == 0)  Serial.print("0000000"); 
    Serial.print(longitude);  
    Serial.write("_");   
    
    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
}

bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
//  if(!AT_CWSAP("WIFI_HOTSPOT","1234567890"))  return false;
  if(!AT_CWJAP("hotspot","1234567890"))
  {   Serial.println("Not Connected to server");
      return false;
  }
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
  data = readData(5000);
  if(compare_string(data,(char *)"WIFI DISCONNECT",15)) return(0);
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14); 
  return(return_flag);
}

bool AT_test(void)
{ String data = "";
  Serial.println(F("AT"));
  data = readData(1000);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ Serial.println(F("AT+RESTORE"));
  readData(3000);
}

void AT_RST(void)
{ Serial.println(F("AT+RST"));
  readData(1000);
}

void AT_CWMODE(char Mode)
{ Serial.print(F("AT+CWMODE="));
  Serial.println(Mode,DEC);
  readData(1000);
}
void AT_CIPMUX(void)
{ String data = "";
  Serial.println(F("AT+CIPMUX=1"));
  data = readData(1000);
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
  data = readData(1000);
  return(compare_string(data,(char *)"OK",2));    
}

bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial.print(F("AT+CIPCLOSE="));
  Serial.println(linkID);
  data = readData(1000);
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
    char i = 0;
    unsigned long t = millis();
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
//--------------------------------------------------------
