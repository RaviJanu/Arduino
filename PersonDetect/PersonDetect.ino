//!!AUM!!
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t getFingerprintEnroll();

#define KEY_1   0xE1
#define KEY_2   0xE2
#define KEY_3   0xE4
#define KEY_A   0xE8
#define KEY_4   0xD1
#define KEY_5   0xD2
#define KEY_6   0xD4
#define KEY_B   0xD8
#define KEY_7   0xB1
#define KEY_8   0xB2
#define KEY_9   0xB4
#define KEY_C   0xB8
#define KEY_S   0x71
#define KEY_0   0x72
#define KEY_H   0x74
#define KEY_D   0x78

#define MAX_STUDENT             100
#define CONSTANT                16


int crnt_match_point = 0;
char browser_LinkID = 0, App_LinkID = 0, Module_LinkID = 0;
char AppContDataSending = 0;
int ContSending_timer = 0;

uint8_t id,send_id = 0;;
byte check_attendance = 0;
int base_ms = 0;
const byte Key_Row[4] = {A4,A5,8,9};
const byte Key_Col[4] = {A0,A1,A2,A3};
byte Key_Val = 0;
byte key_num;
byte key_col_sel = 0;
byte avoid_sence_cnt = 0,avoid_sence_flag = 0;
char registeration_flag = 0;


void setup()  
{ char i = 0;
  for(i = 0;i<4;i++)
  { pinMode(Key_Col[i],OUTPUT);
    digitalWrite(Key_Col[i],HIGH);
    pinMode(Key_Row[i],INPUT_PULLUP);
  }
  
  Serial.begin(9600); 
  pinMode(KEY_1,INPUT_PULLUP);
  pinMode(KEY_2,INPUT_PULLUP);
  pinMode(KEY_3,INPUT_PULLUP);
  pinMode(KEY_4,INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   Biometric    ");    
  lcd.setCursor(0, 1);
  lcd.print("AttendanceSystem");  

  delay(5000);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (!finger.verifyPassword()) 
  {   lcd.setCursor(0, 0);
      lcd.print(" FINGER PRINT   ");    
      lcd.setCursor(0, 1);
      lcd.print(" MODULE ERROR   "); 
      delay(3000);
  }  
  
  if(!AT_test())   
  { Serial.println(F("Check ESP Module and Restart Arduino"));
  }
  if(!Server_setup())
  { Serial.println(F("Server Not Setup")); 
  }
}

void loop()
{ key_sence();
  key_action();  
  if(key_num == 14)   
  { if(check_attendance == 0)     check_attendance = 1;
    else                          check_attendance = 0;
  }
  if(key_num == 15)               register_student_id();    //registeration_flag = 1; 

  GET_NEW_DATA();
  
  delay(1);
  base_ms++;
  if(base_ms < 1000)  return;
  base_ms = 0;

  if(check_attendance == 1)     
  { lcd.setCursor(0, 0);
    lcd.print("SCAN YOUR FINGER");    
    lcd.setCursor(0, 1);
    lcd.print("                "); 
    check_for_attendance();
  }
  else
  { lcd.setCursor(0, 0);
    lcd.print("   Biometric    ");    
    lcd.setCursor(0, 1);
    lcd.print("AttendanceSystem"); 
  }
  if(registeration_flag == 1)   register_student_id();
//  Send_Continously();    
}


void key_sence(void)
{   char i = 0;
    byte bit_tbl[4] = {0xFE,0xFD,0xFB,0xF7};

    Key_Val = 0x00;
    
   if(avoid_sence_flag == 1)
   {  avoid_sence_cnt++;
      if(avoid_sence_cnt < 250)  return;
      avoid_sence_cnt = 0; 
      avoid_sence_flag = 0;
   }

   for(i = 0;i<4;i++)
   {  digitalWrite(Key_Col[i],HIGH);
   } 
   digitalWrite(Key_Col[key_col_sel],LOW);
   Key_Val =  bit_tbl[key_col_sel] << 4;    
   
   for(i = 0;i<4;i++)
   {  if(digitalRead(Key_Row[i]) == 0)
      { Key_Val |= ~bit_tbl[i];
        break;
      }
   }

   if( (Key_Val & 0x0f) != 0x00)    avoid_sence_flag = 1;
   key_col_sel++;
   if(key_col_sel > 3)  key_col_sel = 0;
}

void key_action(void)
{ key_num = CONSTANT;
  if((Key_Val & 0x0f) == 0)     return;
  switch(Key_Val)
  { case KEY_1:   key_num = 1;
    break;
    case KEY_2:   key_num = 2;
    break;
    case KEY_3:   key_num = 3;
    break;
    case KEY_4:   key_num = 4;
    break;
    case KEY_5:   key_num = 5;
    break;
    case KEY_6:   key_num = 6;
    break;
    case KEY_7:   key_num = 7;
    break;
    case KEY_8:   key_num = 8;
    break;
    case KEY_9:   key_num = 9;
    break;
    case KEY_0:   key_num = 0;
    break;
    case KEY_A:   key_num = 10;
    break;
    case KEY_B:   key_num = 11;
    break;
    case KEY_C:   key_num = 12;
    break;
    case KEY_D:   key_num = 13;
    break;
    case KEY_S:   key_num = 14;
    break;
    case KEY_H:   key_num = 15;
    break;
  }
}


int check_for_attendance(void)
{ 
  int p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER)  return p;
  char student_match_id = 0;
  byte  i = 0,id_number = 0;
//--------------------------------------------------------------------------------
  p = finger.image2Tz(2);
//---------------------------------------------------------------
  for(i = 1;i< MAX_STUDENT;i++)
  { p = finger.loadModel(i);
    p = finger.createModel();
    if(p == FINGERPRINT_OK)
    { id_number = i;
      break;
    }
  }

  if(id_number != 0)
  { send_id = id_number;
    lcd.setCursor(0, 1);
    lcd.print("PERSON ID: "+String(id_number) + "      ");  
    if(AppContDataSending == 1)   AppData_Send(App_LinkID);
    else                          Serial.println("PERSON ID "+String(id_number));   
    delay(5000);
    return  0;
   }
    lcd.setCursor(0, 1);
    lcd.print("PERSON NOT MATCH");    
    delay(3000);
}

int register_student_id(void)
{ id = readnumber();
//---GET IMAGE---------------------------------------------------
  int p = -1;
  lcd.setCursor(0, 1);
  lcd.print("TAP FINGER ONCE ");      
  while (p != FINGERPRINT_OK) 
  { p = finger.getImage();
    delay(50);
  }
  // OK success!
 //------CONVERT IMAGE--------------------------------------------------------------------------  
  p = finger.image2Tz(1);

 //----WAIT FOR REMOVE FINGER--------------------------------------------------------------- 
  lcd.setCursor(0, 1);
  lcd.print("REMOVE FINGER   ");      
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//-------TAB AGAIN---------------------------------------------------------------
  lcd.setCursor(0, 1);
  lcd.print("TAP FINGER AGAIN");   
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
  lcd.setCursor(0, 1);
  lcd.print("RegistrationDone");                                                                      //  CC2500
  check_attendance = 0;
  registeration_flag = 0;  
  
//    if(AppContDataSending == 1)   
//      { send_id = id;
//        AppData_Send(App_LinkID);
//      }
//    else                          Serial.println("PERSON ID "+String(id));   

  delay(3000);
}

uint8_t readnumber(void) 
{ uint8_t num = 0;
  
  lcd.setCursor(0, 0);
  lcd.print("PERSON ID "+String(num)+"     ");      
  lcd.setCursor(0, 1);
  lcd.print("  REGISTRATION  ");
  delay(1000);
  
  while (1) 
  { key_sence();
    key_action();

    if(key_num < 10)
    { num = key_num + num*10;
      if(num >= MAX_STUDENT)  num = 0;
      lcd.setCursor(0, 0);
      lcd.print("PERSON ID "+String(num)+"     ");      
    }
    if(key_num == 13) 
    { num = 0;
      lcd.setCursor(0, 0);
      lcd.print("PERSON ID "+String(num)+"     ");  
    }
    if(key_num == 10) return num;
    
    delay(1);
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
{ String data = "ID000_";
  int string_size = data.length();
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.print("ID");                                                                               //  CC2500
    if(send_id < 100) Serial.print('0');                                                          //  CC2500
    if(send_id < 10)  Serial.print('0');                                                           //  CC2500
    Serial.print(send_id,DEC);     
    Serial.print("_");
    
    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
}

bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
  if(!AT_CWSAP("WIFI_HOTSPOT","1234567890"))  return false;
/*  
  if(!AT_CWJAP("hotspot","1234567890"))
  {   Serial.println("Not Connected to server");
      return false;
  }*/
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











