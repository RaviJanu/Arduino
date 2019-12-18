
//!!AUM!!
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define KEY_1   A1
#define KEY_2   A2
#define KEY_3   A3
#define KEY_4   A4


#define MAX_STUDENT             5
#define MAX_STUDENT_NAME_SIZE   15
uint8_t id;

char check_attendance = 0;
int base_ms = 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t getFingerprintEnroll();

void setup()  
{ Serial.begin(9600); 

  pinMode(KEY_1,INPUT_PULLUP);
  pinMode(KEY_2,INPUT_PULLUP);
  pinMode(KEY_3,INPUT_PULLUP);
  pinMode(KEY_4,INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("  FINGER PRINT  ");    
  lcd.setCursor(0, 1);
  lcd.print("EXAMINATN SYSTEM");  
  delay(3000);

  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (!finger.verifyPassword()) 
  {   lcd.setCursor(0, 0);
      lcd.print(" FINGER PRINT   ");    
      lcd.setCursor(0, 1);
      lcd.print(" MODULE ERROR   "); 
    while (1);
  }  
  
}


void loop()
{ if(!digitalRead(KEY_1))        register_student_id();
  if(!digitalRead(KEY_4))
  { if(check_attendance == 0)     check_attendance = 1;
    else                          check_attendance = 0;
    //Serial.println("Read to student Attandence");
    delay(500);
  }

  delay(1);
  base_ms++;
  if(base_ms < 1000)  return;
  base_ms = 0;

  if(check_attendance == 1) 
  { lcd.setCursor(0, 0);
    lcd.print("MARK ATTENDANCE ");    
    lcd.setCursor(0, 1);
    lcd.print("                ");
    check_for_attendance();
  }
  else    
  { lcd.setCursor(0, 0);
    lcd.print("  FINGER PRINT  ");    
    lcd.setCursor(0, 1);
    lcd.print("EXAMINATN SYSTEM");  
  }
}


int check_for_attendance(void)
{ 
  int p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER)  return p;
  char student_match_id = 0;
  char  i = 0,id_number = 0;
//--------------------------------------------------------------------------------
  p = finger.image2Tz(2);
//---------------------------------------------------------------
  for(i = 1;i<=MAX_STUDENT;i++)
  { p = finger.loadModel(i);
    p = finger.createModel();
    if(p == FINGERPRINT_OK)
    { id_number = i;
      break;
    }
  }

  if(id_number != 0)
  { Serial.print("ID");                                                                              //  CC2500
    if(id_number < 10) Serial.print('0');                                                           //  CC2500
    Serial.println(id_number,DEC);  
    lcd.setCursor(0, 1);
    lcd.print("STUDENT ID:     ");  
    lcd.setCursor(12, 1);
    lcd.print(id_number,DEC);
    delay(5000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    return  0;
   }
    lcd.setCursor(0, 1);
    lcd.print("Student NotMatch");    
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    
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
  delay(2000);
}

uint8_t readnumber(void) 
{ uint8_t num = 1;
  
  lcd.setCursor(0, 0);
  lcd.print("STUDENT ID      ");      
  lcd.setCursor(0, 1);
  lcd.print("  REGISTRATION  ");
  delay(1000);
  
  while (1) 
  { lcd.setCursor(12, 0);
    lcd.print("   ");
    lcd.setCursor(12, 0);
    lcd.print(num,DEC); 
    
    if(!digitalRead(KEY_1))        return num;
    if(!digitalRead(KEY_2))        
    {  if(num < MAX_STUDENT)     num++;
       else             num = 1;
    }
    if(!digitalRead(KEY_3)) 
    {  if(num > 1)      num--;
       else             num = MAX_STUDENT;
    }
    
    delay(300);
 }
}

















