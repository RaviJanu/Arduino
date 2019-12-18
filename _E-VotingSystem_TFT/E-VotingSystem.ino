//!!AUM!!
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>     // Touch library
#include <Adafruit_Fingerprint.h>     //Finger print library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS      A3  // Chip Select goes to Analog 3
#define LCD_CD      A2  // Command/Data goes to Analog 2
#define LCD_WR      A1  // LCD Write goes to Analog 1
#define LCD_RD      A0  // LCD Read goes to Analog 0
#define LCD_RESET   A4  // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define TFT_BLACK   0x0000
#define TFT_BLUE    0x001F
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Calibrates value
#define SENSIBILITY 300
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//These are the pins for the shield!
#define YP A1 
#define XM A2 
#define YM 7  
#define XP 6 

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
#define HRES              320 /* Default screen resulution for X axis */
#define VRES              240 /* Default screen resulution for Y axis */
// Init TouchScreen:
TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2); //finger_print module


#define MAX_PERSON        8 
#define LINE_1_ADDR       40
#define LINE_2_ADDR       LINE_1_ADDR + 30
#define LINE_3_ADDR       LINE_2_ADDR + 30
#define LINE_4_ADDR       LINE_3_ADDR + 30
#define LINE_5_ADDR       LINE_4_ADDR + 30

#define TIME_OUT          600  //60*10 SEC

#define KEY_1   22
#define KEY_2   24
#define KEY_3   26
#define KEY_4   28

byte check_attendance = 0,Matched_Id = 0;
int base_ms = 0;

unsigned long Timer_Counter = 0;
byte verified_person[] = {0,0,0,0,0,0,0,0};


///240 x 320 resolution tft lcd

void setup() 
{ tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial

  Serial1.begin(9600);  //CC2500
  Serial.begin(9600);  
  pinMode(KEY_1,INPUT_PULLUP);
  pinMode(KEY_2,INPUT_PULLUP);
  pinMode(KEY_3,INPUT_PULLUP);
  pinMode(KEY_4,INPUT_PULLUP);

  
  Basic_Slides(1," ",TFT_BLACK);
  Basic_Slides(2,"    RVOTER",TFT_BLACK);
  Basic_Slides(3,"  GPM COLLEGE",TFT_BLACK);
  delay(2000);
  
  finger.begin(57600);
  if (!finger.verifyPassword()) 
  {  Basic_Slides(5,"FINGER PRINT ERROR",TFT_BLACK);
     delay(2000);
  }
}
char step_num = 0;


void loop() 
{  // put your main code here, to run repeatedly:
  if(!digitalRead(KEY_1))        Fp_register();
  if(!digitalRead(KEY_2))
  { check_attendance = 0;   
    delay(500);
  }  
  if(!digitalRead(KEY_3))
  { check_attendance = 1;   
    delay(500);
  }  
  if(!digitalRead(KEY_4))
  { check_attendance = 2;  
    delay(500);
  }  
  
  delay(1);
  base_ms++;
  if(base_ms < 1000)  return;
  base_ms = 0;
  
  Timer_Counter = micros();
  Timer_Counter /= 1000000;
  if(Timer_Counter < TIME_OUT)
  { if(step_num == 0)
    { Basic_Slides(1,"VERIFY YOUR BIOMETRIC",TFT_BLACK);
      Basic_Slides(2,"GIVE YOUR FINGERPRINT",TFT_BLACK);
      Basic_Slides(5,"         Time: " + String(Timer_Counter) + "sec",TFT_BLACK);
      check_for_fingerprint();
    }
  }
  else
  {   Serial1.print("X");
      Serial.print("X");
      while(1)
      { Timer_Counter = micros();
        Timer_Counter /= 1000000;
        Basic_Slides(1," ",TFT_BLACK);
        Basic_Slides(2,"VOTING TIME OUT",TFT_BLACK);
        Basic_Slides(5,"         Time:" + String(Timer_Counter) + " sec",TFT_BLACK);
        delay(1000);
      }
  }
  
}


void Basic_Slides(char Line_num,String PrintData,int Color)
{ if(Line_num == 1)
  { tft.fillScreen(TFT_WHITE);
    tft.drawRect(10,10,HRES-20,VRES-20,TFT_BLACK);
  }

  tft.setTextColor(Color);      
//  tft.setTextFont(2);        
  tft.setTextSize(2); 
  if(Line_num == 1) tft.setCursor (40,LINE_1_ADDR);
  if(Line_num == 2) tft.setCursor (40,LINE_2_ADDR);
  if(Line_num == 3) tft.setCursor (40,LINE_3_ADDR);
  if(Line_num == 4) tft.setCursor (40,LINE_4_ADDR);
  if(Line_num == 5) tft.setCursor (40,LINE_5_ADDR);
 
  tft.print(PrintData);
}

#define MAIN_REC_BODER    2
#define MAIN_START_X      4
#define MAIN_START_Y      4
#define NAME_REC_WIDTH    155 
#define NAME_REC_HIGHT    46

#define CURSOR_POINT      10


byte Sence_Candidate =0;
const int X_LimitArray[3] = {0,155,320},Y_LimitArray[5] = {46,46*2,46*3,46*4,46*5};
const char candidate_name[8][15] = {"1> B.J.P.","5> I.N.C.","2> B.S.P.","6> N.C.P.","3> M.N.S.","7> S.S.","4> A.A.P.","8> No Option"};

void Main_Slide(void)
{ char i = 0,j = 0,count = 0;
  Sence_Candidate = 0;  

  tft.fillScreen(TFT_WHITE);
  tft.drawRect(MAIN_REC_BODER,MAIN_REC_BODER,HRES-MAIN_REC_BODER*2,VRES-MAIN_REC_BODER*2,TFT_RED);

  tft.setTextColor(TFT_BLACK);      
  tft.setTextSize(3); 
  tft.drawRect(MAIN_START_X,MAIN_START_Y,NAME_REC_WIDTH*2,NAME_REC_HIGHT,TFT_BLUE);
  tft.setCursor(MAIN_START_X+CURSOR_POINT,MAIN_START_Y+CURSOR_POINT);
  tft.print("CANDIDATE OPTION");
  
  tft.setTextSize(2); 
  for(i = 1;i<5;i++)
  { for(j = 0;j<2;j++)
    {   tft.drawRect(MAIN_START_X+NAME_REC_WIDTH*j,MAIN_START_Y+NAME_REC_HIGHT*i,NAME_REC_WIDTH,NAME_REC_HIGHT,TFT_BLUE);
        tft.setCursor(MAIN_START_X+NAME_REC_WIDTH*j+CURSOR_POINT,MAIN_START_Y+NAME_REC_HIGHT*i+CURSOR_POINT);
        tft.print(candidate_name[count]); 
        count++;
    }
  }
  while(1)
  { Sence_Candidate = TouchScreen();
    if(Sence_Candidate != 0)  return;
    
  }
}

byte TouchScreen(void)
{ digitalWrite(13, HIGH);             //for touch screen this pin should be init 
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
  { p.x = map(p.x,1000, 100,0,320);
    p.y = map(p.y,900, 100,0,240 );
    
//   Serial.print("X = "); Serial.print(p.x);
//    Serial.print("\tY = "); Serial.print(p.y);
//    Serial.print("\tPressure = "); Serial.println(p.z);

    
    char i = 0;
    byte returnID = 0;
    
    for(i = 1;i < 5;i++)
    { if((p.x > X_LimitArray[0])&&(p.x < X_LimitArray[1])&&(p.y > Y_LimitArray[i-1])&&(p.y < Y_LimitArray[i])) 
      { returnID = i;
        break;
      }
      if((p.x > X_LimitArray[1])&&(p.x < X_LimitArray[2])&&(p.y > Y_LimitArray[i-1])&&(p.y < Y_LimitArray[i])) 
      { returnID = i+4;
        break;
      }
    }
    return(returnID);
//    Serial.println("ID : "+String(Sence_Candidate));
//    delay(500);
  } return(0);
}


//------Finger Print Code--------------------------------
int check_for_fingerprint(void)
{ 
  int p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER)  return p;
  char student_match_id = 0;
  byte  i = 0,id_number = 0;
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
  { Matched_Id = id_number;
    step_num = 1;
    if(verified_person[i-1] == 1) 
    { Basic_Slides(1,"PERSON ALREADY VOTED",TFT_BLACK); 
      Basic_Slides(2,"VOTE NOT COUNT",TFT_BLACK);   
    }
    else
    { Main_Slide();
      Serial1.println("ID" + String(id_number)+"CT" + String(Sence_Candidate) + "_\n");
      Serial.println("ID" + String(id_number)+"CT" + String(Sence_Candidate) + "_");
      verified_person[i-1] = 1;
    }
  }
  else
  { Basic_Slides(1,"NOT A VALID PERSON",TFT_BLACK);    
  }
  step_num = 0;
    
}

int Fp_register(void)
{ uint8_t id;
  id = readnumber();
//---GET IMAGE--------------
  int p = -1;
  Basic_Slides(1,"1st PUT FINGER",TFT_BLACK);
  while (p != FINGERPRINT_OK) 
  { p = finger.getImage();
    delay(50);
  }
  // OK success!
//------CONVERT IMAGE-------
  p = finger.image2Tz(1);
//----WAIT FOR REMOVE FINGER
  Basic_Slides(2,"REMOVE FINGER",TFT_BLACK);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//-------TAB AGAIN------------------------------
  Basic_Slides(3,"PUT FINGER AGAIN",TFT_BLACK);
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
  Basic_Slides(4,"REGISTRATION DONE",TFT_BLACK);
  step_num = 0;
  delay(2000);
}

uint8_t readnumber(void) 
{ uint8_t num = 1;
 
  Basic_Slides(1,"SET USER ID FOR",TFT_BLACK); 
  Basic_Slides(2,"FIRST REGISTRATION",TFT_BLACK); 
  Basic_Slides(3,"ID NUMBER : "+String(num),TFT_BLACK); 
  
  delay(1000);
  while (1) 
  { if(!digitalRead(KEY_1))        return num;
    if(!digitalRead(KEY_2))        
    {  if(num < MAX_PERSON)     num++;
       else                     num = 1;
       Basic_Slides(1,"SET USER ID FOR",TFT_BLACK); 
       Basic_Slides(2,"FIRST REGISTRATION",TFT_BLACK); 
       Basic_Slides(3,"ID NUMBER : "+String(num),TFT_BLACK); 
    }
    if(!digitalRead(KEY_3)) 
    {  if(num > 1)              num--;
       else                     num = MAX_PERSON;
       Basic_Slides(1,"SET USER ID FOR",TFT_BLACK); 
       Basic_Slides(2,"FIRST REGISTRATION",TFT_BLACK); 
       Basic_Slides(3,"ID NUMBER : "+String(num),TFT_BLACK); 
    }
    delay(300);
 }
}




