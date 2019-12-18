//!!AUM!!
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
#include <LiquidCrystal.h>
// RS-2  ; EN-3 ; D4-4    ;  D5-5   ; D6-6    ; D7-8
LiquidCrystal lcd(2, 3, 4, 5, 6, 8);

#define THUMB_FLEX    0
#define INDEX_FLEX    1
#define MIDDLE_FLEX   2
#define RING_FLEX     3
#define PINKY_FLEX    4
#define X_ACC         0
#define Y_ACC         1

#define MAX_FIFO_BUFFER   10

int  FLEX_ARRAY[5] = {0,0,0,0,0};
int  FIFO_BUFFER[5][MAX_FIFO_BUFFER],ADC_FIFO[2][MAX_FIFO_BUFFER];
long FLEX_DATA[10],ADC_DATA[2];
int  ACC_ADC[2];

struct  word_signal
{ char  Word[17];
  char  SendChar;
  byte  open_close_bit[10];
  byte  angle[2];  
};
byte Angle_int_to_nibble[2] = {0,0},Flex_int_to_bit[10] = {0,0,0,0,0,0,0,0,0,0};

/*
 * 
"Hello"                                   //TESTED
"Help"                                    //TESTED
"Thankyou"                                //TESTED
"Sorry"                                   //TESTED
"Goodbye"                                 //TESTED
"Our"                                     //TESTED
"Vocally challenge people"    //TESTED
"Nice"    //TESTED
"This is"
"Project"
"Designed"
"To"
"Have a"
"Day"
*/
#define MAX_SIGN    18
const struct word_signal  FindGesture[20] = 
{ " "               , '0' , {1,1,1,1,1,1,1,1,1,1} , {2,2},
  "HELLO"           , 'A' , {1,0,0,0,0,0,0,0,0,0} , {0,2},      //testt
  "GOODBYE"         , 'B' , {1,0,0,0,0,0,0,0,0,0} , {0,1},      //testt
  "OUR"             , 'C' , {0,0,0,0,0,0,0,0,0,0} , {1,1},       //TEST
  "SORRY"           , 'L' , {1,0,0,0,1,0,0,0,0,0} , {1,0},     //testt
  "THANK YOU"       , 'M' , {1,0,0,0,0,0,0,0,0,0} , {0,0},     //testt
  "HELP"            , 'G' , {0,0,0,0,1,0,0,0,0,0} , {1,0},      //testt
  "VocalyChlngePple", 'D' , {0,0,0,0,1,0,0,1,1,1} , {1,1},   //testt   
  "NICE"            , 'E' , {0,0,0,0,1,0,0,1,1,1} , {1,0},      //testt
  "THIS IS"         ,  'N', {1,0,0,0,1,0,0,1,1,1} , {1,0},    
  "TO"              , 'H' , {0,0,0,0,0,0,0,1,1,0} , {0,0},      
  "DAY"             , 'F' , {0,0,0,0,0,0,0,0,0,0} , {0,0},      
  "PROJECT"         , 'K' , {0,0,0,0,1,0,0,0,0,0} , {1,1},  
  "HAVE A"          , 'I' , {1,0,0,0,0,0,0,0,0,0} , {1,0},      
  "DESIGNED"        , 'J' , {0,0,0,0,0,0,0,1,1,1} , {0,1},          
  "I"               , 'O' , {1,0,0,0,1,0,0,0,0,0} , {2,0},      //testt
  "clear screen"    , 'X' , {1,0,0,0,1,0,0,1,1,1} , {0,1},      //testt
};


void setup() 
{ // put your setup code here, to run once:
  Serial.begin(9600); 
  mySerial.begin(9600); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" SIGN LANGUAGE  ");    
  lcd.setCursor(0, 1);
  lcd.print("                ");  
}


unsigned int base_ms = 0;
byte listen_timer = 0;
byte match_history = 0;

void loop() 
{ byte i =0,j = 0;
  FLEX_ARRAY[THUMB_FLEX]   = analogRead(THUMB_FLEX);
  FLEX_ARRAY[INDEX_FLEX]   = analogRead(INDEX_FLEX);
  FLEX_ARRAY[MIDDLE_FLEX]  = analogRead(MIDDLE_FLEX);
  FLEX_ARRAY[RING_FLEX]    = 400;                    
//FLEX_ARRAY[RING_FLEX]    = analogRead(RING_FLEX);
  FLEX_ARRAY[PINKY_FLEX]   = analogRead(PINKY_FLEX-1);  
  ACC_ADC[X_ACC]           = analogRead(X_ACC+4);
  ACC_ADC[Y_ACC]           = analogRead(Y_ACC+4);


   for(i = 0; i < 5; i++)
  {   for(j = 0;j< MAX_FIFO_BUFFER-1;j++)
      { FIFO_BUFFER[i][j] = FIFO_BUFFER[i][j+1];
        FLEX_DATA[i] += FIFO_BUFFER[i][j];
        if(i<2)
        { ADC_FIFO[i][j] = ADC_FIFO[i][j+1];
          ADC_DATA[i] += ADC_FIFO[i][j];
        }
      }
  }
  for(i=0;i<5;i++)
  { FIFO_BUFFER[i][MAX_FIFO_BUFFER-1] = FLEX_ARRAY[i];
    FLEX_DATA[i] += FIFO_BUFFER[i][MAX_FIFO_BUFFER-1];
    FLEX_DATA[i] = FLEX_DATA[i]/MAX_FIFO_BUFFER;
    if(i < 2)      
    {  ADC_FIFO[i][MAX_FIFO_BUFFER-1]  = ACC_ADC[i];
       ADC_DATA[i] += ADC_FIFO[i][MAX_FIFO_BUFFER-1];
       ADC_DATA[i] = ADC_DATA[i]/MAX_FIFO_BUFFER;
    } 
  }

  if(Serial.available())     readData(); 
  
  delay(10);
  base_ms++;
  if(base_ms < 100) return;
  base_ms = 0;
  
  Serial.println(String(FLEX_DATA[0])+"_"+String(FLEX_DATA[1])+"_"+String(FLEX_DATA[2])+"_"+String(FLEX_DATA[3])+"_"+String(FLEX_DATA[4])+"_"+String(ADC_DATA[0])+" "+String(ADC_DATA[1]));
  Find_Sign();

  listen_timer++;
  if(listen_timer > 10) 
  { listen_timer = 10;
    Flex_int_to_bit[5] = 0;
    Flex_int_to_bit[6] = 0;
    Flex_int_to_bit[7] = 0;
    Flex_int_to_bit[8] = 0;
    Flex_int_to_bit[9] = 0;
  }
}



void Find_Sign(void)
{  byte i = 0,j = 0,j_match = 0;
   byte match_char = 0;
   simplify_signal();
   Flex_int_to_bit[1] = 0;
   for(i = 0;i<MAX_SIGN ;i++)
   {  j_match = 0;
      for(j = 0;j<10;j++)
     {   if((Flex_int_to_bit[j] == FindGesture[i].open_close_bit[j]))     
         {  j_match++;
//            Serial.print(j_match);
         }
     }
//     Serial.println(i);
     if((j_match == 10)&&(Angle_int_to_nibble[X_ACC] == FindGesture[i].angle[X_ACC])&&(Angle_int_to_nibble[Y_ACC] == FindGesture[i].angle[Y_ACC]))
     {  match_char = i+1;
        break;
     }
   }
//   Serial.println(match_char);Serial.println(match_history);
   if((match_char == match_history)&&(match_char > 0))
   {  lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(FindGesture[match_char-1].Word); 
      Serial.println(FindGesture[match_char-1].SendChar); 
      mySerial.println(FindGesture[match_char-1].SendChar); 
      delay(1000);
   }
   else
   {  lcd.setCursor(0, 1);
      lcd.print("                "); 
   }
   match_history = match_char;
}

void simplify_signal(void)
{ int Xangle_thresh_UM = 340,Xangle_thresh_MD = 400,Yangle_thresh_0_90 = 340,Yangle_thresh_90_180 = 400;
  int ThumbThresh = 600,IndexThresh = 1400,MidleThresh = 1400,PinkyThresh = 500;

  if(ADC_DATA[X_ACC] < Xangle_thresh_UM)                                                Angle_int_to_nibble[X_ACC] = 0;
  if((ADC_DATA[X_ACC] > Xangle_thresh_UM)&&(ADC_DATA[X_ACC] < Xangle_thresh_MD))        Angle_int_to_nibble[X_ACC] = 1;
  if(ADC_DATA[X_ACC] > Xangle_thresh_MD)                                                Angle_int_to_nibble[X_ACC] = 2;
  if(ADC_DATA[Y_ACC] < Yangle_thresh_0_90)                                              Angle_int_to_nibble[Y_ACC] = 0;
  if((ADC_DATA[Y_ACC] > Yangle_thresh_0_90)&&(ADC_DATA[Y_ACC] < Yangle_thresh_90_180))  Angle_int_to_nibble[Y_ACC] = 1;
  if(ADC_DATA[Y_ACC] > Yangle_thresh_90_180)                                            Angle_int_to_nibble[Y_ACC] = 2;

  if(FLEX_DATA[THUMB_FLEX] < ThumbThresh)                                               Flex_int_to_bit[THUMB_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[THUMB_FLEX] = 1;
  if(FLEX_DATA[INDEX_FLEX] < IndexThresh)                                               Flex_int_to_bit[INDEX_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[INDEX_FLEX] = 1;
  if(FLEX_DATA[MIDDLE_FLEX] < MidleThresh)                                              Flex_int_to_bit[MIDDLE_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[MIDDLE_FLEX] = 1;
  Flex_int_to_bit[RING_FLEX] = 0;
  if(FLEX_DATA[PINKY_FLEX] < PinkyThresh)                                               Flex_int_to_bit[PINKY_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[PINKY_FLEX] = 1;


    
  lcd.setCursor(0, 0);
  lcd.print("               ");
  for(char i =0;i<10;i++)
  {  Serial.print(String(Flex_int_to_bit[i])+"_");
      lcd.setCursor(i, 0);
     lcd.print(Flex_int_to_bit[i]);
  }
  Serial.print(String(Angle_int_to_nibble[X_ACC])+"_");
  Serial.println(String(Angle_int_to_nibble[Y_ACC])+"_");
  
  lcd.setCursor(11, 0);
  lcd.print(Angle_int_to_nibble[X_ACC]);
  lcd.print(Angle_int_to_nibble[Y_ACC]);

}

void readData(void) 
{   unsigned long t = millis();
    int i = 0;
    char Rx_Data[5];
    while(millis() - t < 100) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read(); 
          if(i < 5) Rx_Data[i] = r;
//          Serial.print(Rx_Data[i]);
          i++;
      }
    }
//    Serial.println("_");
    Flex_int_to_bit[THUMB_FLEX+5]   = Rx_Data[THUMB_FLEX] - 0x30;
    Flex_int_to_bit[INDEX_FLEX+5]   = Rx_Data[INDEX_FLEX] - 0x30;
    Flex_int_to_bit[MIDDLE_FLEX+5]  = Rx_Data[MIDDLE_FLEX] - 0x30;
    Flex_int_to_bit[RING_FLEX+5]    = Rx_Data[RING_FLEX] - 0x30;
    Flex_int_to_bit[PINKY_FLEX+5]   = Rx_Data[PINKY_FLEX] - 0x30;
    listen_timer = 0;
}


