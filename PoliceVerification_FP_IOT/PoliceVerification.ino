//!!AUM!!
//Fingerprint sensor, IOT, 
#include <avr/pgmspace.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define KEY_1           A0
#define KEY_2           A1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2); //finger_print module

#define MAX_PERSON              5
#define INPUT_DATA_BUFF_SIZE    50

int crnt_match_point = 0;
char browser_LinkID = 0,App_LinkID = 0 ; 
char AppContDataSending = 0;
int ContSending_timer = 0;

int base_ms = 0;
byte check_attendance = 0;
byte SEND_ID = 0;

void setup() 
{  // put your setup code here, to run once:
  Serial.begin(115200);     //uart
  Serial3.begin(115200);     //uart

  pinMode(KEY_1,INPUT_PULLUP);
  pinMode(KEY_2,INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);  
  lcd.print("DIGITAL POLICE  ");   
  lcd.setCursor(0, 1);  
  lcd.print(" VERIFICATION   ");  
  
  delay(5000);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (!finger.verifyPassword()) 
  { Serial.print("FP SENSOR ERROR "); 
    lcd.setCursor(0, 1);  
    lcd.print(" FP MODULE ERR  "); 
    delay(2000);
  }
  
  if(!AT_test())   
  { lcd.setCursor(0, 1);  
    lcd.print("WIFI MODULE ERR "); 
    delay(2000);
  }
  if(!client_setup())   
  { lcd.setCursor(0, 1);  
    lcd.print("CLIENT CONN ERR "); 
    delay(2000);
  }
}

void loop() 
{ // put your main code here, to run repeatedly:
  GET_NEW_DATA(); 
  if(!digitalRead(KEY_1))        Fp_register();
  if(!digitalRead(KEY_2))
  { if(check_attendance == 0) check_attendance = 1;
    else                      check_attendance = 0;
    Serial.println("Check Verifircation : " + String(check_attendance));
    delay(500);
  }  
  
  delay(1);
  base_ms++;
  if(base_ms < 1000)  return;
  base_ms = 0;

  if(check_attendance == 1)
  { check_for_attendance();
    lcd.setCursor(0, 0);  
    lcd.print("MARK FINGERPRINT");   
    lcd.setCursor(0, 1);  
    lcd.print("                ");  
  }
  else
  { lcd.setCursor(0, 0);  
    lcd.print("DIGITAL POLICE  ");   
    lcd.setCursor(0, 1);  
    lcd.print(" VERIFICATION   ");     
  }
//  Send_Continously();
}

//------Finger Print Code--------------------------------
int check_for_attendance(void)
{ int p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER)  return p;
  char student_match_id = 0;
  char  i = 0,id_number = 0;
//--------------------------------------------------------------------------------
  p = finger.image2Tz(2);
//---------------------------------------------------------------
  for(i = 1;i<=MAX_PERSON;i++)
  { p = finger.loadModel(i);
    p = finger.createModel();
    if(p == FINGERPRINT_OK)
    { id_number = i;
      break;
    }
  }

  if(id_number != 0)
  { SEND_ID = id_number;
    Serial.println("Person ID: "+String(SEND_ID)); 
    lcd.setCursor(0, 0);  
    if(SEND_ID >= 2)     lcd.print(" PERSON ID : "+String(SEND_ID)+"  ");  
    else                 lcd.print(" PERSON ID : "+String(SEND_ID)+"  ");   
    lcd.setCursor(0, 1);  
    lcd.print(" MATCHED  ");  
    delay(3000);
    SEND_DATA();
  } 
  else
  { lcd.setCursor(0, 0);  
    lcd.print("    PERSON ID   ");   
    lcd.setCursor(0, 1);  
    lcd.print("   NOT MATCHED  ");  
    Serial.println("ID NOT MATCHED");
  }
}


int Fp_register(void)
{ unsigned int id =0 ;
  id = readnumber();
//---GET IMAGE---------------------------------------------------
  int p = -1;
  Serial.println("TAP FINGER ONCE ");      
  while (p != FINGERPRINT_OK) 
  { p = finger.getImage();
    delay(50);
  }
  // OK success!
 //------CONVERT IMAGE--------------------------------------------------------------------------  
  p = finger.image2Tz(1);
 //----WAIT FOR REMOVE FINGER--------------------------------------------------------------- 
  Serial.println("REMOVE FINGER   ");      
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//-------TAB AGAIN---------------------------------------------------------------
  Serial.println("TAP FINGER AGAIN");   
  p = -1;
  while (p != FINGERPRINT_OK) 
  { p = finger.getImage();     
  }
  // OK success! 
//--------------------------------------------------------------------------------
  p = finger.image2Tz(2);
  p = finger.createModel();   
//-------STORE FINGER IMAGE-----------------------------------------------------------  
  p = finger.storeModel(id);
  Serial.println("RegistrationDone");    
  delay(2000);
}


uint8_t readnumber(void) 
{ uint8_t num = 1;
  Serial.println("Select ID:" + String(num));
  delay(1000);
  while (1) 
  { if(!digitalRead(KEY_1))        return num;
    if(!digitalRead(KEY_2))        
    {  if(num < MAX_PERSON)     num++;
       else             num = 1;
       Serial.println(num);
    }
    delay(300);
 }
}

//----------Wifi Code-------------------------------------

void Send_Continously(void)
{ ContSending_timer++;
  if(ContSending_timer < 10) return;
  ContSending_timer = 0;

  SEND_DATA();
}

void SEND_DATA(void)
{ if(!AT_CIPSTART())
  {  lcd.setCursor(0, 0);
     lcd.print(F("TCP CONNECT ERR"));
     Serial.println("TCP CONNECT ERR");
     delay(1000); 
     return;
  }
  
  if(AT_CIPSEND())
  { lcd.setCursor(0, 0);
    lcd.print(F(" DATA SEND      "));
     Serial.println("DATA SEND");
     delay(1000); 
  }
  else
  { lcd.setCursor(0, 0);
    lcd.print(F("DATA SENDING ERR"));
     Serial.println("DATA SENDING ERR");
    delay(1000);
  }
//  AT_CIPCLOSE();
}


void GET_NEW_DATA(void)
{ if (Serial3.available() == 0) return;

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
  if(!AT_CWJAP("hotspot","1234567890"))
  {   Serial.println("Not Connected to server");
      return false;
  }
  return true;  
}



void AT_CIPCLOSE(void)
{ String data = "";
  Serial3.println("AT+CIPCLOSE=1");
  data = readData(1000);
}

bool AT_CIPSTART(void)
{ String data = "";
  bool return_flag = 0;
  Serial3.println("AT+CIPSTART=1,\"TCP\",\"192.168.43.56\",80");
  data = readData(5000);
  return_flag = compare_string(data,(char *)"1,CONNECT",9); 
  return(return_flag);
}
//http://localhost/1332/test.php?ID=15
bool AT_CIPSEND(void)
{ String Senddata = "GET /1332/test.php?ID=" + String(SEND_ID) + " HTTP/1.1\r\nHost: 192.168.43.56\r\n\r\n";
  int Str_length = Senddata.length();
  String data = "";
  Serial3.print("AT+CIPSEND=1,");
  Serial3.println(Str_length,DEC);
  data = readData(2000);
  if(compare_string(data,(char *)">",1))
  { Serial3.println(Senddata);
    data = readData(10000);
    if(compare_string(data,(char *)"SEND OK",7))
    { return true;
    }
  }
  return false;
}



bool AT_CWJAP(String ipaddr,String passwrd)
{ String data = "";
  bool return_flag = 0;
  Serial3.print("AT+CWJAP=\"");
  Serial3.print(ipaddr);
  Serial3.print("\",\"");
  Serial3.print(passwrd);
  Serial3.println("\"");
  data = readData(10000);
  if(compare_string(data,(char *)"WIFI DISCONNECT",15)) return(0);
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14); 
  return(return_flag);
}

bool AT_test(void)
{ String data = "";
  Serial3.println(F("AT"));
  data = readData(1000);
  return(compare_string(data,(char *)"OK",2));
}

void AT_RESTORE(void)
{ Serial3.println(F("AT+RESTORE"));
  readData(5000);
}

void AT_RST(void)
{ Serial3.println(F("AT+RST"));
  readData(1000);
}

void AT_CWMODE(char Mode)
{ Serial3.print(F("AT+CWMODE="));
  Serial3.println(Mode,DEC);
  readData(1000);
}
void AT_CIPMUX(void)
{ String data = "";
  Serial3.println(F("AT+CIPMUX=1"));
  data = readData(1000);
}

bool AT_CWSAP(String My_WifiName,String My_Wifipswd)
{ String data = "";
  Serial3.print(F("AT+CWSAP=\""));
  Serial3.print(My_WifiName);
  Serial3.print(F("\",\""));
  Serial3.print(My_Wifipswd);
  Serial3.println(F("\",5,3"));
  data = readData(5000);
  return (compare_string(data, (char *)"OK", 2));
}

bool AT_CIPSERVER(int port_num)
{ String data = "";
  Serial3.print(F("AT+CIPSERVER=1,"));
  Serial3.println(port_num,DEC);
  data = readData(1000);
  return(compare_string(data,(char *)"OK",2));    
}

bool AT_CIPCLOSE(char linkID)
{ String data = "";
  Serial3.print(F("AT+CIPCLOSE="));
  Serial3.println(linkID);
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
    unsigned long t = millis();
    while(millis() - t < timeout) 
    { if(Serial3.available() > 0) 
      {   char r = Serial3.read();
          data += r;  
          t = millis();
      }
    }
    Serial.println(data);
    return data;
}
//--------------------------------------------------------
