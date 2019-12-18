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

#define MAX_FIFO_BUFFER   10
int  FLEX_ARRAY[5] = {0,0,0,0,0};
int  FIFO_BUFFER[5][MAX_FIFO_BUFFER];
long FLEX_DATA[10];

void setup() 
{ // put your setup code here, to run once:
  Serial.begin(9600); 
  mySerial.begin(9600); 
}



unsigned int base_ms = 0;

void loop() 
{ byte i =0,j = 0;
  FLEX_ARRAY[THUMB_FLEX]   = analogRead(THUMB_FLEX);
  FLEX_ARRAY[INDEX_FLEX]   = analogRead(INDEX_FLEX);
  FLEX_ARRAY[MIDDLE_FLEX]  = analogRead(MIDDLE_FLEX);             
  FLEX_ARRAY[RING_FLEX]    = analogRead(RING_FLEX);
  FLEX_ARRAY[PINKY_FLEX]   = analogRead(PINKY_FLEX); 

   for(i = 0; i < 5; i++)
  {   for(j = 0;j< MAX_FIFO_BUFFER-1;j++)
      { FIFO_BUFFER[i][j] = FIFO_BUFFER[i][j+1];
        FLEX_DATA[i] += FIFO_BUFFER[i][j];
      }
  }
  for(i=0;i<5;i++)
  { FIFO_BUFFER[i][MAX_FIFO_BUFFER-1] = FLEX_ARRAY[i];
    FLEX_DATA[i] += FIFO_BUFFER[i][MAX_FIFO_BUFFER-1];
    FLEX_DATA[i] = FLEX_DATA[i]/MAX_FIFO_BUFFER;
  }
  delay(10);
  base_ms++;
  if(base_ms < 100) return;
  base_ms = 0;
  
//  Serial.println(String(FLEX_DATA[0])+"_"+String(FLEX_DATA[1])+"_"+String(FLEX_DATA[2])+"_"+String(FLEX_DATA[3])+"_"+String(FLEX_DATA[4]));
  simplify_signal();
}


byte Angle_int_to_nibble[2] = {0,0},Flex_int_to_bit[10] = {0,0,0,0,0,0,0,0,0,0};
void simplify_signal(void)
{ int ThumbThresh = 1140,IndexThresh = 1200,MidleThresh = 890,RingThresh = 1100,PinkyThresh = 980;

  if(FLEX_DATA[THUMB_FLEX] < ThumbThresh)                                               Flex_int_to_bit[THUMB_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[THUMB_FLEX] = 1;
  if(FLEX_DATA[INDEX_FLEX] < IndexThresh)                                               Flex_int_to_bit[INDEX_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[INDEX_FLEX] = 1;
  if(FLEX_DATA[MIDDLE_FLEX] < MidleThresh)                                              Flex_int_to_bit[MIDDLE_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[MIDDLE_FLEX] = 1;
  if(FLEX_DATA[RING_FLEX] < RingThresh)                                                 Flex_int_to_bit[RING_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[RING_FLEX] = 1;
  if(FLEX_DATA[PINKY_FLEX] < PinkyThresh)                                               Flex_int_to_bit[PINKY_FLEX] = 0;
  else                                                                                  Flex_int_to_bit[PINKY_FLEX] = 1;

//  Serial.println("___"+String(Flex_int_to_bit[THUMB_FLEX])+"_"+String(Flex_int_to_bit[INDEX_FLEX])+"_"+String(Flex_int_to_bit[MIDDLE_FLEX])+"_"+String(Flex_int_to_bit[RING_FLEX])+"_"+String(Flex_int_to_bit[PINKY_FLEX]));
  Serial.println(String(Flex_int_to_bit[THUMB_FLEX])+String(Flex_int_to_bit[INDEX_FLEX])+String(Flex_int_to_bit[MIDDLE_FLEX])+String(Flex_int_to_bit[RING_FLEX])+String(Flex_int_to_bit[PINKY_FLEX]));

}














