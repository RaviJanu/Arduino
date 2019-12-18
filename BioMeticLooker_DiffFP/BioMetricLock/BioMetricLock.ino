//!!AUM!!
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#include "FPS_GT511C3.h"
#include <SoftwareSerial.h>
// set up software serial pins for Arduino's w/ Atmega328P's
// FPS (TX) is connected to pin 4 (Arduino's Software RX)
// FPS (RX) is connected through a converter to pin 5 (Arduino's Software TX)
FPS_GT511C3 fps(12, 13); // (Arduino SS_RX = pin 4, Arduino SS_TX = pin 5)


#define MAX_STUDENT     4
#define MOTOR_1         8
#define MOTOR_2         9

#define KEY_1           A0
#define KEY_2           A1
#define KEY_3           A2
#define KEY_4           A3

#define INPUT_DATA_BUFF_SIZE 50
int crnt_match_point = 0;
char App_LinkID = 0; 
char AppContDataSending = 0;
int ContSending_timer = 0;

char IdentifyFP_flag = 0;
unsigned int base_ms = 0;
byte VerifiedID = 0;

void setup() 
{ Serial.begin(115200); //set up Arduino's hardware serial UART
  pinMode(KEY_1,INPUT_PULLUP);
  pinMode(KEY_2,INPUT_PULLUP);
  pinMode(KEY_3,INPUT_PULLUP);
  pinMode(KEY_4,INPUT_PULLUP);
  pinMode(MOTOR_1,OUTPUT);
  pinMode(MOTOR_2,OUTPUT);
  digitalWrite(MOTOR_1,LOW);
  digitalWrite(MOTOR_2,LOW);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   BIO-METIC    ");    
  lcd.setCursor(0, 1);
  lcd.print("UnLockingof Labs");  
  
  delay(3000);
  if(!AT_test())   
  { Serial.println("WifiModule Error"); 
    lcd.setCursor(0, 1);
    lcd.print("WifiModule Error");
  }
  if(!Server_setup())  
  { Serial.println("Connecion Error ");
    lcd.setCursor(0, 1);
    lcd.print("Connecion Error ");
    delay(1000);
  }
  fps.Open();         //send serial command to initialize fps
  fps.SetLED(true);   //turn on LED so fps can see fingerprint
}

void loop() 
{ GET_NEW_DATA();
  if(!digitalRead(KEY_1))        Enroll();
  if(!digitalRead(KEY_4))
  { if(IdentifyFP_flag == 0)     IdentifyFP_flag = 1;
    else                         IdentifyFP_flag = 0;
    delay(500);
  }
  delay(1);
  
  base_ms++;
  if(base_ms < 1000)  return;
  base_ms = 0;

  if(IdentifyFP_flag == 1) 
  { lcd.setCursor(0, 0);
    lcd.print("MARK FINGERPRINT");    
    lcd.setCursor(0, 1);
    lcd.print("                ");
    IdentifyFP();
  }
  else    
  { lcd.setCursor(0, 0);
    lcd.print("   BIO-METIC    ");    
    lcd.setCursor(0, 1);
    lcd.print("UnLockingof Labs");  
  }
}


void IdentifyFP(void)
{ // Identify fingerprint test
  if (!fps.IsPressFinger())     return;
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200) //<- change id value depending model you are using
    {//if the fingerprint matches, provide the matching template ID
      lcd.setCursor(0, 0);
      lcd.print(" Finger Matched ");
      lcd.setCursor(0, 1);
      lcd.print("Verified ID: "+String(id)+"   ");
      //Serial.print("Verified ID:");
      //Serial.println(id);
      delay(3000);
      VerifiedID = id;
      if(!AppData_Send(App_LinkID))
      { lcd.setCursor(0, 0);
        lcd.print("DataSendingError");
      }
      else
      { lcd.setCursor(0, 0);
        lcd.print(" Data Send      ");
      }
    }
    else
    { //if unable to recognize
      //Serial.println("Finger not found");
      lcd.setCursor(0, 0);
      lcd.print("Finger Not Match");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    delay(2000);
}


void Enroll()
{ // Enrollment 
  // find open enroll id
  lcd.setCursor(0, 0);
  lcd.print("ENROLL NEW FP-ID");      
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  int enrollid = getEnrolId();
  fps.EnrollStart(enrollid);

  // enroll
  lcd.setCursor(0, 1);
  lcd.print("PresFinger2Enrol");
  //Serial.print("Press finger to Enroll #");
  //Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  
  lcd.setCursor(0, 1);
  lcd.print(" Remove finger  ");
  if (bret != false)
  { //Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    
    lcd.setCursor(0, 1);
    lcd.print("PressFingerAgain");
    //Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    { lcd.setCursor(0, 1);
      lcd.print(" Remove finger  ");
      //Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      lcd.setCursor(0, 1);
      lcd.print("PressFingerAgain");
      //Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      { lcd.setCursor(0, 1);
        lcd.print(" Remove finger  ");
        //Serial.println("Remove finger");
        delay(1000);
        iret = fps.Enroll3();
        if (iret == 0)
        { lcd.setCursor(0, 1);
          lcd.print("Enroll Succesful");
          //Serial.println("Enrolling Successful");
        }
        else
        { lcd.setCursor(0, 1);
          lcd.print(" Enroll Failed  ");
          //Serial.print("Enrolling Failed with error code:");
          //Serial.println(iret);
        }
        delay(1000);
      }
      else 
      { //Serial.println("Failed to capture third finger");
        lcd.setCursor(0, 1);
        lcd.print("Fail2Capture3Tim");
      }
    }
    else 
    { //Serial.println("Failed to capture second finger");
      lcd.setCursor(0, 1);
      lcd.print("Fail2Capture2Tim");
    }
  }
  else 
  { //Serial.println("Failed to capture first finger");
    lcd.setCursor(0, 1);
    lcd.print("Fail2Capture1Tim");
  }
  delay(2000);
}


byte getEnrolId(void) 
{ byte num = 1;
   
  lcd.setCursor(0, 1);
  lcd.print("Set Enroll ID:"+String(num));
  delay(1000);
  while (1) 
  { if(!digitalRead(KEY_2))        
    {  if(num < MAX_STUDENT)     num++;
       else             num = 1;  
       lcd.setCursor(0, 1);
       lcd.print("Set Enroll ID:"+String(num));
    }
    if(!digitalRead(KEY_3)) 
    {  if(num > 1)      num--;
       else             num = MAX_STUDENT;
       lcd.setCursor(0, 1);
       lcd.print("Set Enroll ID:"+String(num));
    }
    if(!digitalRead(KEY_1))        return num;
    delay(300);
 }
}


void GET_NEW_DATA(void)
{  if (Serial.available() == 0) return;

  String Data = "";
  Data = readData(1000);
  byte temp = 0;
  char str_to_char_buff[INPUT_DATA_BUFF_SIZE]; 
  Data.toCharArray(str_to_char_buff,50);
  
  if(compare_string(Data, (char *)"+IPD,",5))
  {   temp =  crnt_match_point;
      
      lcd.setCursor(0, 1);
      lcd.print(str_to_char_buff);
      delay(2000);
      if(compare_string(Data, (char *)"PP_",3))
      { App_LinkID = str_to_char_buff[temp];
//        AppData_Send(App_LinkID);
        AppContDataSending = 1;     
        if(str_to_char_buff[crnt_match_point] == 'Q')
        { lcd.setCursor(0, 1);
          lcd.print("  DOOR OPEN     ");
          digitalWrite(MOTOR_1,HIGH);
          digitalWrite(MOTOR_2,LOW);
          delay(800);  
          digitalWrite(MOTOR_1,LOW);
          digitalWrite(MOTOR_2,LOW);
          while(1)
          { if(!digitalRead(KEY_2)) break;
          }
          lcd.setCursor(0, 1);
          lcd.print("  DOOR CLOSE    ");
          digitalWrite(MOTOR_2,HIGH);
          digitalWrite(MOTOR_1,LOW);
          delay(800);  
          digitalWrite(MOTOR_1,LOW);
          digitalWrite(MOTOR_2,LOW);
        }
        return;
      }
 
  }
  if(compare_string(Data, (char *)"CLOSED",6))
  { if(str_to_char_buff[crnt_match_point-8] == App_LinkID)  AppContDataSending = 0;
  }
}

bool AppData_Send(char Link_ID)
{ String Senddata = "ID"+String(VerifiedID)+"_";
  int string_size = Senddata.length();
  String data = "";
  
  Serial.print("AT+CIPSEND=");
  Serial.print(Link_ID);
  Serial.print(",");
  Serial.println(string_size,DEC);
  data = readData(2000);

  if (compare_string(data, (char *)">", 1))
  { Serial.println(Senddata);
    data = readData(2000);
    if(compare_string(data,(char *)"SEND OK", 7)) return true;
  }
  return false;
}

bool Server_setup(void)
{ AT_RESTORE();
  AT_CWMODE(3);
  AT_CIPMUX();
//  if(!AT_CWSAP("MY_HOTSPOT","1234567890"))  return false;
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
  data = readData(10000);
  if(compare_string(data,(char *)"WIFI DISCONNECT",15)) return(0);
  return_flag = compare_string(data,(char *)"WIFI CONNECTED",14); 
  return(return_flag);
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
    char i =0;
    unsigned long t = millis();
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          if(i < 50)  data += r;
          i++;  
          t = millis();
      }
    }
    return data;
}





