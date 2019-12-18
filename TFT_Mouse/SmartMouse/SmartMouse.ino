
#define PORTRAIT  0
#define LANDSCAPE 1

#define TOUCH_ORIENTATION  PORTRAIT
#define TITLE "TouchScreen.h GFX Calibration"

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// MCUFRIEND UNO shield shares pins with the TFT.
int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
#include "TouchScreen_kbv.h"         //my hacked version
TouchScreen_kbv ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint_kbv tp;                            //global point

#define TFT_BLACK   ~0x0000
#define TFT_BLUE    ~0x001F
#define TFT_RED     ~0xF800
#define TFT_GREEN   ~0x07E0
#define TFT_CYAN    ~0x07FF
#define TFT_MAGENTA ~0xF81F
#define TFT_YELLOW  ~0xFFE0
#define TFT_WHITE   ~0xFFFF

#define HRES              480 /* Default screen resulution for X axis */
#define VRES              320 /* Default screen resulution for Y axis */
#define LINE_1_ADDR       50
#define LINE_2_ADDR       LINE_1_ADDR + 30
#define LINE_3_ADDR       LINE_2_ADDR + 30
#define LINE_4_ADDR       LINE_3_ADDR + 30
#define LINE_5_ADDR       LINE_4_ADDR + 30

void readResistiveTouch(void)
{   tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);  //because TFT control pins
    digitalWrite(XM, HIGH);
}

bool ISPRESSED(void)
{   // .kbv this was too sensitive !!
    // now touch has to be stable for 50ms
    int count = 0;
    bool state, oldstate;
    while (count < 10) {
        readResistiveTouch();
        state = tp.z > 20;
        if (state == oldstate) count++;
        else count = 0;
        oldstate = state;
        delay(5);
    }
    return oldstate;
}

int dispx, dispy, text_y_center, swapxy;

boolean diagnose_pins()
{   int i, j, value, Apins[2], Dpins[2], Values[2], found = 0;
    for (i = A0; i < A5; i++) pinMode(i, INPUT_PULLUP);
    for (i = 2; i < 10; i++) pinMode(i, INPUT_PULLUP);
    for (i = A0; i < A4; i++) 
    {   pinMode(i, INPUT_PULLUP);
        for (j = 5; j < 10; j++) 
        { pinMode(j, OUTPUT);
          digitalWrite(j, LOW);
          value = analogRead(i);               // ignore first reading
          value = analogRead(i);
          if (value < 100 && value > 0) 
          {   if (found < 2) 
              { Apins[found] = i;
                Dpins[found] = j;
                Values[found] = value;
              }
              found++;
          }
          pinMode(j, INPUT_PULLUP);
        }
        pinMode(i, INPUT_PULLUP);
    }
    if (found == 2) 
    {//   Serial.println(F("Diagnosing as:-"));
        int idx = Values[0] < Values[1];
        XM = Apins[!idx]; XP = Dpins[!idx]; YP = Apins[idx]; YM = Dpins[idx];
        ts = TouchScreen_kbv(XP, YP, XM, YM, 300);    //re-initialise with pins
        return true;                              //success
    }
    Serial.println(F("BROKEN TOUCHSCREEN"));
    return false;
}

void setup()
{   Serial.begin(9600);
    Serial1.begin(9600);
    bool ret = true;
    ret = diagnose_pins();
    uint16_t ID = tft.readID();
//    Serial.print("ID = 0x");
//    Serial.println(ID, HEX);
    tft.begin(ID);
    tft.setRotation(1);

    Basic_Slides(1," ",TFT_BLACK);
    Basic_Slides(4,"  SMART BOARD",TFT_BLACK);
    delay(2000);
    Mouse_Buttion();
}

void loop()
{  if(ISPRESSED() == true) calibrate(10, 10, 0);
}



#define Right_Button_X    390
#define Right_Button_Y    20
#define Right_Button_W    80
#define Right_Button_H    145

#define Left_Button_X     Right_Button_X
#define Left_Button_Y     160
#define Left_Button_W     Right_Button_W
#define Left_Button_H     Right_Button_H

void calibrate(int x, int y, int i)
{   readResistiveTouch();
    int temp = tp.x;
    tp.x = map(tp.y,960, 100,0,480);
    tp.y = map(temp,900,130 ,0,320);
//    Serial.println("X"+String(tp.x)+"Y"+String(tp.y)+"_");
    if(tp.x < Right_Button_X)
    { tp.x = map(tp.x,0,Right_Button_X,0,1024);
      tp.y = map(tp.y,0,320,0,768);  
      Serial1.println("X"+String(tp.x)+"Y"+String(tp.y)+"_");
      Serial.println("X"+String(tp.x)+"Y"+String(tp.y)+"_");
    }
    else
    { if((tp.y > Right_Button_Y)&&(tp.y < (Right_Button_Y+Right_Button_H)))   
      { Serial1.println("R");     delay(1000);
        Serial.println("R");
      }
      if((tp.y > Left_Button_Y)&&(tp.y < (Left_Button_Y+Left_Button_H)))   
      { Serial1.println("L");     delay(1000);
        Serial.println("L");
      }
    }
}


void Mouse_Buttion(void)
{ tft.drawRect(Right_Button_X,Right_Button_Y,Right_Button_W - 5,Right_Button_H - 5,TFT_BLACK);
  tft.drawRect(Left_Button_X,Left_Button_Y,Left_Button_W - 5,Left_Button_H - 5,TFT_BLACK);

  tft.setTextSize(2); 
  tft.setCursor (Right_Button_X + 10,Right_Button_Y+50);
  tft.print("RIGHT");
  tft.setCursor (Left_Button_X + 10,Left_Button_Y+50);
  tft.print("LEFT");
}


void Basic_Slides(char Line_num,String PrintData,int Color)
{ if(Line_num == 1)
  { tft.fillScreen(TFT_WHITE);
    tft.drawRect(10,10,HRES-20,VRES-20,TFT_BLACK);
  }
  tft.setTextColor(Color);      
  tft.setTextSize(3); 
  if(Line_num == 1) tft.setCursor (70,LINE_1_ADDR);
  if(Line_num == 2) tft.setCursor (70,LINE_2_ADDR);
  if(Line_num == 3) tft.setCursor (70,LINE_3_ADDR);
  if(Line_num == 4) tft.setCursor (70,LINE_4_ADDR);
  if(Line_num == 5) tft.setCursor (70,LINE_5_ADDR);
 
  tft.print(PrintData);
}


