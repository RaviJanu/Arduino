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

#define SWITCH        9

#define MAX_FINGER    5
#define MAX_CHAR      5
int FLEX_ARRAY[] = {0,0,0,0,0,0,0,0,0,0};
unsigned int base_ms = 0;
const int CHAR_REFF_ARRAY[MAX_CHAR][MAX_FINGER] = 
{ {780,1023,810,750,755},     //A
  {756,810,820,840,816},     //B
  {755,834,850,875,833},     //C
  {790,1023,846,865,827},     //D
  {0,0,0,0,0},     //E
};
char char_match[] = "ABCDE";


void setup() 
{ // put your setup code here, to run once:
  pinMode(SWITCH,INPUT_PULLUP);
  Serial.begin(9600); 
  mySerial.begin(9600); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" SIGN LANGUAGE  ");    
  lcd.setCursor(0, 1);
  lcd.print("                ");  
  
}


void loop() 
{ FLEX_ARRAY[THUMB_FLEX]   = analogRead(THUMB_FLEX);
  FLEX_ARRAY[INDEX_FLEX]   = analogRead(INDEX_FLEX);
  FLEX_ARRAY[MIDDLE_FLEX]  = analogRead(MIDDLE_FLEX);
  FLEX_ARRAY[RING_FLEX]    = analogRead(RING_FLEX);
  FLEX_ARRAY[PINKY_FLEX]   = analogRead(PINKY_FLEX);

//  Serial.println("T"+String(FLEX_ARRAY[THUMB_FLEX])+"I"+String(FLEX_ARRAY[INDEX_FLEX])+"M"+String(FLEX_ARRAY[MIDDLE_FLEX])+"R"+String(FLEX_ARRAY[RING_FLEX])+"P"+String(FLEX_ARRAY[PINKY_FLEX]));
 // Serial.write('@');   
//  if(Serial.available())  readData(100); 
  if(digitalRead(SWITCH) == 0)
  { for(char i = 0;i<MAX_CHAR;i++)
    {   byte find_char = 0;
        for(char j = 0;j<MAX_FINGER;j++)
        {   int itemp = 0;
            itemp = FLEX_ARRAY[j] - CHAR_REFF_ARRAY[i][j];
            if((itemp > -50)&&(itemp < 50))   find_char++;
        }
        if(find_char == 4)
        { Serial.print(char_match[i]);
          lcd.setCursor(0, 1);
          lcd.print(char_match[i]);  
          break;
        }
    }
  }

  
  delay(100);
  base_ms++;
  if(base_ms < 15)    return;
  base_ms = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");  
}

void readData(unsigned long timeout) 
{   char *data;
    unsigned long t = millis();
    int i = 0;
    while(millis() - t < timeout) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read(); 
          if((r >= 0x30)&&(r <= 0x39))      data[i] = r;  
          t = millis();
          Serial.println(data[i]); 
          i++;
      }
    }
    asciitoint(data);
}

void asciitoint(char *data_string)
{ char unit = 0,dec = 0,hundred = 0;

  hundred = *data_string - 0x30;    
  data_string++;
  dec     = *data_string - 0x30;   
  data_string++;
  unit    = *data_string - 0x30;    
  data_string++;
  FLEX_ARRAY[5] = hundred*100 + dec*10 + unit;
  Serial.println(FLEX_ARRAY[5],DEC);
}



