//!!AUM!!
#define LOAD_SENSOR     A0
#define DOOR_LIMIT_SW   2
#define EGG1_SW         3
#define EGG2_SW         4
#define DOOR_SW         5
#define DOOR_MOTOR_1    6
#define DOOR_MOTOR_2    7
#define BUZZER          8

#define INPUT_DATA_BUFF_SIZE    50
#define BUFF_MAX                100
int     Adc_buff[BUFF_MAX];


int crnt_match_point = 0;
char browser_LinkID = 0,App_LinkID = 0,Module_LinkID = 0; 
char AppContDataSending = 0;
int ContSending_timer = 0;
unsigned char wifi_error_flag = 0;

int base_ms=0;

void setup() 
{ pinMode(DOOR_LIMIT_SW,INPUT_PULLUP);
  pinMode(EGG1_SW,INPUT_PULLUP); 
  pinMode(EGG2_SW,INPUT_PULLUP); 
  pinMode(DOOR_SW,INPUT_PULLUP); 
  pinMode(DOOR_MOTOR_1,OUTPUT); 
  pinMode(DOOR_MOTOR_2,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(DOOR_MOTOR_1,LOW);
  digitalWrite(DOOR_MOTOR_2,LOW);
  digitalWrite(BUZZER,HIGH);

  delay(5000);
  Serial.begin(115200);
  Serial1.begin(115200);

  if(!AT_test())   
  { Serial.print("WIFI MODULE ERR "); 
  }
  if(!client_setup())   
  { Serial.print("CLIENT CONN ERR "); 
  }
}


byte egg1_status = 0,egg2_status = 0,door_status = 0,door_flag = 0,door_counter = 0;
unsigned int load_value = 0,Send_Load_value = 0;

void loop() 
{ int i = 0; 
  long ADC_Avg = 0;
  unsigned int analog_value = analogRead(LOAD_SENSOR);
  for(i = 0; i < BUFF_MAX-1 ; i++)
  {   Adc_buff[i] = Adc_buff[i+1];
      ADC_Avg += Adc_buff[i]; 
  }
  Adc_buff[BUFF_MAX-1] = analog_value;
  ADC_Avg += Adc_buff[BUFF_MAX-1];
  ADC_Avg /= BUFF_MAX;
  load_value = ADC_Avg;
  
  
  if(!digitalRead(DOOR_SW))       
  { if(door_flag == 0)
    { door_flag = 1;
      digitalWrite(DOOR_MOTOR_1,HIGH);
      digitalWrite(DOOR_MOTOR_2,LOW);
      delay(800);
      digitalWrite(DOOR_MOTOR_1,LOW);
      digitalWrite(DOOR_MOTOR_2,LOW);
      Serial.println("Door Open");
    } 
    else
    { door_flag = 0;
      digitalWrite(DOOR_MOTOR_2,HIGH);
      digitalWrite(DOOR_MOTOR_1,LOW);
      delay(800);
      digitalWrite(DOOR_MOTOR_1,LOW);
      digitalWrite(DOOR_MOTOR_2,LOW);
      Serial.println("Door Close");
    }  
  }

  
  GET_NEW_DATA();
  delay(10);
  base_ms++;
  if(base_ms < 100) return;
  base_ms = 0;
  if(load_value < 230)  Send_Load_value = map(load_value , 0 , 200 , 2 , 0);
  else                  Send_Load_value = 0;
  
  if(digitalRead(DOOR_LIMIT_SW))    door_status = 1;
  else                              
  { door_status = 0;  
    digitalWrite(BUZZER,HIGH);
  }

  if(digitalRead(EGG1_SW))          egg1_status = 1;
  else                              egg1_status = 0;
  
  if(digitalRead(EGG2_SW))          egg2_status = 1;
  else                              egg2_status = 0;

  if(door_status == 1)
  { door_counter++;
    if(door_counter > 15)
    { door_counter = 0;
      door_flag = 0;
      digitalWrite(DOOR_MOTOR_2,HIGH);
      digitalWrite(DOOR_MOTOR_1,LOW);
      delay(800);
      digitalWrite(DOOR_MOTOR_1,LOW);
      digitalWrite(DOOR_MOTOR_2,LOW);
      Serial.println("Door Close");
    }  
    if(door_counter > 5)      digitalWrite(BUZZER,LOW);
  }  
  else  door_counter = 0;

  Serial.println("D=" +String(door_status)+ "E="+ String(egg1_status)+ "G="+String(egg2_status)+ "L="+String(load_value)+"O="+String(Send_Load_value));
  Send_Continously();
}

void Send_Continously(void)
{ ContSending_timer++;
  if(ContSending_timer < 10) return;
  ContSending_timer = 0;

  SEND_DATA();
}



void SEND_DATA(void)
{ if(!AT_CIPSTART())
  {  Serial.print(F("TCP CONNECT ERR"));
     return;
  }
  if(AT_CIPSEND())
  { Serial.print(F(" DATA SEND      "));
  }
  else
  { Serial.print(F("DATA SENDING ERR"));
  }
//  AT_CIPCLOSE();
}


void GET_NEW_DATA(void)
{ if (Serial1.available() == 0) return;
  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE]; 
  Data.toCharArray(str_to_char_buff,50);

  if(compare_string(Data, (char *)"+IPD,",5))
  { temp =  crnt_match_point;
    if(compare_string(Data, (char *)"PP_",4))
    { App_LinkID = str_to_char_buff[temp];
      return;
     }    
  }
  
  if(compare_string(Data, (char *)"CLOSED",6))
  { if(str_to_char_buff[crnt_match_point-8] == App_LinkID)  AppContDataSending = 0;
  }

}

bool client_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
//  if(!AT_CWSAP("MY_WifiRouter","1234567890"))  return false;
  if(!AT_CWJAP("LENOVO","1234567890"))
  {   Serial1.println("Not Connected to server");
      return false;
  }
  return true;  
}



void AT_CIPCLOSE(void)
{ String data = "";
  Serial1.println("AT+CIPCLOSE=1");
  data = readData(1000);
}

bool AT_CIPSTART(void)
{ String data = "";
  bool return_flag = 0;
  Serial1.println("AT+CIPSTART=1,\"TCP\",\"smartrefrigerator35.000webhostapp.com\",80");
  data = readData(3000);
  return_flag = compare_string(data,(char *)"1,CONNECT",9); 
  return(return_flag);
}
//https://smartrefrigerator35.000webhostapp.com/receive.php?D=1&E=2&G=3&L=4

bool AT_CIPSEND(void)
{ String Senddata = "GET /receive.php?D=" +String(door_status)+ "&E="+ String(egg1_status)+ "&G="+String(egg2_status)+ "&L="+String(Send_Load_value)+" HTTP/1.1\r\nHost: smartrefrigerator35.000webhostapp.com\r\n\r\n";
  int Str_length = Senddata.length();
  String data = "";
  Serial1.print("AT+CIPSEND=1,");
  Serial1.println(Str_length,DEC);
  data = readData(1000);
  if(compare_string(data,(char *)">",1))
  { Serial1.println(Senddata);
    data = readData(10000);
    if(compare_string(data,(char *)"SEND OK",7))
    { return true;
    }
  }
  return false;
}


bool AT_test(void)
{ String data = "";
  Serial1.println(F("AT"));
  data = readData(500);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ Serial1.println(F("AT+RESTORE"));
  readData(2000);
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
{ 
  String data = "";
  Serial1.print(F("AT+CWSAP=\""));
  Serial1.print(My_WifiName);
  Serial1.print(F("\",\""));
  Serial1.print(My_Wifipswd);
  Serial1.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}


bool AT_CWJAP(String ipaddr,String passwrd)
{ 
  String data = "";
  bool return_flag = 0;
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(ipaddr);
  Serial1.print("\",\"");
  Serial1.print(passwrd);
  Serial1.println("\"");
  data = readData(10000);
  return(compare_string(data,(char *)"WIFI CONNECTED",14));
}


bool AT_CIPSERVER(int port_num)
{ 
  String data = "";
  Serial1.print(F("AT+CIPSERVER=1,"));
  Serial1.println(port_num,DEC);
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






