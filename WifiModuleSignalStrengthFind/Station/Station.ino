//!!AUM!!

#define TEMP_SENSOR     A0
#define FIRE_SENSOR     A2
#define LED             13

int crnt_match_point = 0;
char browser_LinkID = 0,App_LinkID = 0; 
char AppContDataSending = 0;
int ContSending_timer = 0;

int base_ms = 0;
byte FireFlag = 0;

void setup() 
{ Serial.begin(115200);     //uart
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  delay(3000);
  if(!AT_test())   
  { Serial.println("WIFI MODULE ERR "); 
  }
  if(!Server_setup())  
  { Serial.println("WifiModule Error");
  }
}


void loop() 
{ int Temp_adc = 0,Fire_adc = 0;
  Temp_adc = analogRead(TEMP_SENSOR);
  Fire_adc = analogRead(FIRE_SENSOR);
  Temp_adc = Temp_adc * 0.48;
  if((Temp_adc > 50)&&(Fire_adc < 300))       
  { digitalWrite(LED,HIGH);
    if(AppContDataSending == 1)       AppData_Send(App_LinkID);
  }
  else   digitalWrite(LED,LOW);
//  Serial.println("T"+String(Temp_adc)+"F"+String(Fire_adc)+"L"+String(FireFlag));
  GET_NEW_DATA();
  delay(1000);
}

void GET_NEW_DATA(void)
{  if (Serial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[50]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      if(compare_string(Data, (char *)"PP_",3))
      { App_LinkID = str_to_char_buff[temp];
        AppContDataSending = 1;       
        delay(10000);
        return;
      }
  }
}


bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWSAP("MY_WifiStation","1234567890"))  return false;
  if (!AT_CIPSERVER(80))                     return false;
  return true;  
}


bool AppData_Send(char Link_ID)
{ String data = "FIR";
  int string_size = data.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.println("FIR");
    data = readData(2000);
    AppContDataSending = 2;
    if(compare_string(data,(char *)"SEND OK", 7)) 
    return true;
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
  readData(2000);
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
  data = readData(5000);
  return(compare_string(data,(char *)"WIFI CONNECTED",14));
}


bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial.print(F("AT+CIPSERVER=1,"));
  Serial.println(port_num,DEC);
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
    char i =0;
    unsigned long t = millis();
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          if(i < 80)  data += r;
          i++;  
          t = millis();
      }
    }
 //   Serial.println(data);
    return data;
}




