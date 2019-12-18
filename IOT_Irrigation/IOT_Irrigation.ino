//!!AUM!!
//DHT11 Sensor, Moisture sensor, IOT
#include <dht.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13);      // RX, TX
dht DHT;

#define DHT11_PIN         A0
#define MOISTURE_SENSOR   A1
#define PUMP_RELAY        2

#define INPUT_DATA_BUFF_SIZE    50
int crnt_match_point = 0;
char App_LinkID = 0; 
char AppContDataSending = 0;
int ContSending_timer = 0;

unsigned int Humidity = 0, Temperature = 0,moisture_flg = 0;
unsigned int base_ms=0;
byte auto_manual_flag = 0;


void setup() 
{ pinMode(DHT11_PIN,INPUT);
  pinMode(MOISTURE_SENSOR,INPUT);
  pinMode(PUMP_RELAY,OUTPUT);
  digitalWrite(PUMP_RELAY,LOW);
  
  Serial.begin(115200);
  mySerial.begin(9600);
  delay(3000);
//  AT_UART(9600);

  if(!AT_test())   
  { Serial.println("WifiModule Error");
  }
  if(!Server_setup())  
  { Serial.println("Connection Error");
  }
}

void loop() 
{ GET_NEW_DATA();
   
  delay(1);
  base_ms++;
  if(base_ms < 1000) return;
  base_ms = 0;
  if(digitalRead(MOISTURE_SENSOR))    
  { moisture_flg = 0;
    if(auto_manual_flag == 0) digitalWrite(PUMP_RELAY,HIGH);
  }
  else               
  { moisture_flg = 1;
    if(auto_manual_flag == 0) digitalWrite(PUMP_RELAY,LOW);
  }
  hum_temp();
//  Serial.println("T: "+String(Temperature)+",H: "+String(Humidity)+",M: "+String(moisture_flg));
  Send_Continously();
}

void hum_temp(void)
{ int chk = DHT.read11(DHT11_PIN);
  Temperature = DHT.temperature;
  Humidity = DHT.humidity;
//  Serial.println("Temp: "+String(Temperature)+ " Hum: "+String(Humidity));
}



void Send_Continously(void)
{ ContSending_timer++;
  if(ContSending_timer < 10) return;
  ContSending_timer = 0;

  if(AppContDataSending == 1)
  {   AppData_Send(App_LinkID);
  }
}


bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWSAP("Irrigation","1234567890"))  return false;
/*  if(!AT_CWJAP("hotspot","1234567890"))
  {   Serial.println("Not Connected to server");
      return false;
  }*/
  if (!AT_CIPSERVER(1234))                     return false;
  return true;
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
        if(str_to_char_buff[crnt_match_point] == 'Q')   
        {  if(auto_manual_flag == 1)                    digitalWrite(PUMP_RELAY,HIGH);
        }
        if(str_to_char_buff[crnt_match_point] == 'W')   
        {  if(auto_manual_flag == 1)                    digitalWrite(PUMP_RELAY,LOW);
        }
        if(str_to_char_buff[crnt_match_point] == 'E')   auto_manual_flag = 0;
        if(str_to_char_buff[crnt_match_point] == 'R')   auto_manual_flag = 1;
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
  String SendData = "T000H000S0_";
  int string_size = SendData.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.write('T');
    if(Temperature < 100)   Serial.write('0');
    if(Temperature < 10)    Serial.write('0');
    Serial.print(Temperature);
    Serial.write('H');
    if(Humidity < 100)   Serial.write('0');
    if(Humidity < 10)    Serial.write('0');
    Serial.print(Humidity);
    Serial.write('S');
    Serial.print(moisture_flg);
    Serial.write('_');
    
    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
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
 //   Serial.println(data);
    return data;
}
