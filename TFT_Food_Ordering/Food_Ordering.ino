//!!AUM!!
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin'
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

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
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// This is the screen size for the raw to coordinate transformation
// width and height specified for landscape orientation
#define HRES 480 /* Default screen resulution for X axis */
#define VRES 320 /* Default screen resulution for Y axis */

#define LINE_1_ADDR       50
#define LINE_2_ADDR       100
#define LINE_3_ADDR       150
#define LINE_4_ADDR       200
#define LINE_5_ADDR       250

#define MENU_SQU        320
#define VERTICAL_NUM    4
#define MENU_REC_HIGHT  70  
#define MENU_REC_WIDTH  MENU_SQU
#define BODER_SPACE     5

#define SIDE_KEY_X      MENU_SQU + BODER_SPACE
#define SIDE_KEY_Y      BODER_SPACE
#define SIDE_KEY_W      140
#define SIDE_KEY_H      50

#define PRICE_BOX_X       SIDE_KEY_X
#define PRICE_BOX_Y       SIDE_KEY_Y
#define PRICE_BOX_W       SIDE_KEY_W
#define PRICE_BOX_H       SIDE_KEY_H + 40

#define ORDER_X         SIDE_KEY_X
#define ORDER_Y         (2*BODER_SPACE + PRICE_BOX_H)
#define ORDER_W         (SIDE_KEY_W/2 - 5)
#define ORDER_H         SIDE_KEY_H

#define CANCEL_X         (SIDE_KEY_X + SIDE_KEY_W/2)
#define CANCEL_Y         (2*BODER_SPACE + PRICE_BOX_H)
#define CANCEL_W         (SIDE_KEY_W/2 - 5)
#define CANCEL_H         SIDE_KEY_H

#define FINAL_X         SIDE_KEY_X
#define FINAL_Y         (ORDER_Y + BODER_SPACE + SIDE_KEY_H)
#define FINAL_W         SIDE_KEY_W
#define FINAL_H         SIDE_KEY_H

#define PLUSE_X         SIDE_KEY_X
#define PLUSE_Y         (FINAL_Y + BODER_SPACE + SIDE_KEY_H)
#define PLUSE_W         (SIDE_KEY_W/2 - 5)
#define PLUSE_H         SIDE_KEY_H

#define MINUS_X         (SIDE_KEY_X + SIDE_KEY_W/2)
#define MINUS_Y         (FINAL_Y + BODER_SPACE + SIDE_KEY_H)
#define MINUS_W         (SIDE_KEY_W/2 - 5)
#define MINUS_H         SIDE_KEY_H

#define PAGEDOWN_X       (SIDE_KEY_X + SIDE_KEY_W/2)
#define PAGEDOWN_Y       (PLUSE_Y + BODER_SPACE + SIDE_KEY_H)
#define PAGEDOWN_W       (SIDE_KEY_W/2 - 5)
#define PAGEDOWN_H       SIDE_KEY_H

#define PAGEUP_X         SIDE_KEY_X 
#define PAGEUP_Y         (PLUSE_Y + BODER_SPACE + SIDE_KEY_H)
#define PAGEUP_W         (SIDE_KEY_W/2 - 5)
#define PAGEUP_H         SIDE_KEY_H



#define MENU_NAME_SIZE  15
#define MAX_MENU  12
#define MAX_PAGE  3

int comper_point = 0;
char process_step = 0,Person_number = 0;
char Order_Value = 0,num_order = 0,page_num = 1; 

char Menu_Name[MAX_MENU][MENU_NAME_SIZE] = {"IDLI","DOSA","VADA PAV","PAV BHAJI","MISAL PAV","SANDWITCH","SAMAOSA","PANI PURI","FRANKEY","KACHORI","BHAJIA","BURGER"};
byte Food_price[MAX_MENU] = {20,35,15,35,35,30,10,15,25,20,15,50};




void setup(void) 
{ Serial.begin(9600);
  Serial.println(F("TFT Init"));
  tft.reset();
  uint16_t identifier = 0x9481;
  
  tft.begin(identifier);
  tft.setRotation(1);

  Basic_Slides(1,"FOOD ORDERING SYSTEM",TFT_BLUE);
  Basic_Slides(2,"St. Xavier's College",TFT_BLUE); 

  delay(3000);
  Draw_Menu();
  
}

void loop() 
{ touch_pad();
  delay(20);
}


#define X_ERR_POINT    340 
#define Y_ERR_POINT    300 
#define X_ERR_WIDTH    120 
#define Y_ERR_HIGHT    30 


#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TS_MINX 90
#define TS_MAXX 940
#define TS_MINY 100
#define TS_MAXY 910

void touch_pad(void)
{ 
  // put your main code here, to run repeatedly:
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
        // if sharing pins, you'll need to fix the directions of the touchscreen pins
    //pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z < MINPRESSURE || p.z > MAXPRESSURE)  // Note this function updates coordinates stored within library variables
    return;
    
    unsigned int X_Raw;
    unsigned int Y_Raw;
    byte point_num = 0,i = 0,j = 0,k = 0;
    int food_price = 0;
 
    // Read the current X and Y axis as raw co-ordinates at the last touch time
    // The values returned were captured when Pressed() was called!
    
    X_Raw = map(p.y, TS_MINX, TS_MAXX, tft.width(), 0);
    Y_Raw = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);
    // Output the results to the Serial11 port 

    for(i = 0;i < VERTICAL_NUM ;i++)
    {     if((X_Raw > 0)&&(X_Raw < MENU_REC_WIDTH)&&(Y_Raw > (MENU_REC_HIGHT*i+40))&&(Y_Raw < (MENU_REC_HIGHT*(i+1)+40)))
          {   point_num = i+1;
              Order_Value = i;
          }
          if((X_Raw > (ORDER_X))&&(X_Raw < (ORDER_X+ORDER_W))&&(Y_Raw > (ORDER_Y))&&(Y_Raw < (ORDER_Y+ORDER_H)))
          { point_num = VERTICAL_NUM+1;
          }
          if((X_Raw > (CANCEL_X))&&(X_Raw < (CANCEL_X+CANCEL_W))&&(Y_Raw > (CANCEL_Y))&&(Y_Raw < (CANCEL_Y+CANCEL_H)))
          { point_num = VERTICAL_NUM+2; 
          }     
          if((X_Raw > (FINAL_X))&&(X_Raw < (FINAL_X+FINAL_W))&&(Y_Raw > (FINAL_Y))&&(Y_Raw < (FINAL_Y+FINAL_H)))
          { point_num = VERTICAL_NUM+7; 
          }  
          
          if((X_Raw > (PLUSE_X))&&(X_Raw < (PLUSE_X+PLUSE_W))&&(Y_Raw > (PLUSE_Y))&&(Y_Raw < (PLUSE_Y+PLUSE_H)))
          { point_num = VERTICAL_NUM+3; 
          }
          if((X_Raw > (MINUS_X))&&(X_Raw < (MINUS_X+MINUS_W))&&(Y_Raw > (MINUS_Y))&&(Y_Raw < (MINUS_Y+MINUS_H)))
          { point_num = VERTICAL_NUM+4; 
          }
          if((X_Raw > (PAGEDOWN_X))&&(X_Raw < (PAGEDOWN_X+PAGEDOWN_W))&&(Y_Raw > (PAGEDOWN_Y))&&(Y_Raw < (PAGEDOWN_Y+PAGEDOWN_H)))
          { point_num = VERTICAL_NUM+5; 
          }
          if((X_Raw > (PAGEUP_X))&&(X_Raw < (PAGEUP_X+PAGEUP_W))&&(Y_Raw > (PAGEUP_Y))&&(Y_Raw < (PAGEUP_Y+PAGEUP_H)))
          { point_num = VERTICAL_NUM+6; 
          }
     }


    Serial.print(" P");
    Serial.println(point_num);   
    

    if(point_num == VERTICAL_NUM+5)     //PAGEDOWN POINT
    { if(page_num < MAX_PAGE) page_num++;
      else                    page_num = 1;
      Order_Value = 0;
    }
    if(point_num == VERTICAL_NUM+6)     //PAGEUP POINT
    { if(page_num > 1)        page_num--;
      else                    page_num = MAX_PAGE;
      Order_Value = 0;
    }

    
    Draw_Menu();
    
    tft.setTextColor(TFT_RED);
    tft.setCursor (PRICE_BOX_X + 5,PRICE_BOX_Y + 5);
    tft.setTextSize(2);


    if(point_num == VERTICAL_NUM+1)     // ORDER POINT
    { tft.print("ORDER");    
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35); 
      tft.print("PLACED");
//      Serial.print("XN_");                      //-------------------------------------VB PART 
      Serial.print(Menu_Name[4*(page_num-1) + Order_Value]);
      delay(2000);
      Serial.print("_");
      Serial.print(Food_price[4*(page_num-1) + Order_Value],DEC); 
       delay(2000);
      Serial.print("_");
      Serial.println(num_order,DEC); 
       delay(2000);
      Order_Value = 0;
    }
    
    if(point_num == VERTICAL_NUM+2)     //CANCEL POINT
    { tft.print("ORDER");
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35); 
      tft.print("CANCELED");    
      Serial.println("OC_");                  //-------------------------------------VB PART 
      Order_Value = 0;
    }
    if(point_num == VERTICAL_NUM+7)     //FINAL POINT
    { tft.print("ORDER");
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35); 
      tft.print("FINAL");    
      Serial.println("OF_");                  //-------------------------------------VB PART 
      Order_Value = 0;
    }

    if((point_num > 0)&&(point_num <= 4))
    { tft.print(Menu_Name[4*(page_num-1) + Order_Value]);
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35); 
      tft.print(Food_price[4*(page_num-1) + Order_Value],DEC);   
      tft.print("Rs");
      num_order = 1;
    }
    
    if(point_num == VERTICAL_NUM+3)     // PLUSE POINT
    { if(num_order < 5) num_order++;
      tft.print(Menu_Name[4*(page_num-1) + Order_Value]);
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35);      
      food_price += Food_price[4*(page_num-1) + Order_Value]*num_order;
      tft.print(food_price,DEC);
      tft.print("Rs");
    }
    
    if(point_num == VERTICAL_NUM+4)     // MINUS POINT
    { if(num_order > 1) num_order--;
      tft.print(Menu_Name[4*(page_num-1) + Order_Value]);
      tft.setCursor (PRICE_BOX_X + 10,PRICE_BOX_Y + 35);      
      food_price += Food_price[4*(page_num-1) + Order_Value]*num_order;
      tft.print(food_price,DEC);
      tft.print("Rs");
    }

    
    
    delay(250); 
}


void Basic_Slides(char Line_num,const char *PrintData,int Color)
{ if(Line_num == 1)
  { tft.fillScreen(TFT_WHITE);
    tft.drawRect(20,20,HRES-40,VRES-40,TFT_BLACK);
  }

  tft.setTextColor(Color);      
  tft.setTextSize(3); 
  if(Line_num == 1) tft.setCursor (50,LINE_1_ADDR);
  if(Line_num == 2) tft.setCursor (50,LINE_2_ADDR);
  if(Line_num == 3) tft.setCursor (50,LINE_3_ADDR);
  if(Line_num == 4) tft.setCursor (50,LINE_4_ADDR);
  if(Line_num == 5) tft.setCursor (50,LINE_5_ADDR);
 
  tft.print(PrintData);
}



 
void Draw_Menu(void)
{ char i = 1,j = 1,k = 0;

  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLUE); // Background is not defined so it is transparent
  tft.setCursor (5,5);     
  tft.setTextSize(3);
  tft.print("MENU CARD,P");
  tft.print(page_num,DEC);

  tft.fillRect(0,40,MENU_REC_WIDTH,MENU_REC_HIGHT*4,TFT_RED);
  for(i = 0;i < VERTICAL_NUM;i++)
  {  tft.drawRect(0,40+MENU_REC_HIGHT*i,MENU_REC_WIDTH,MENU_REC_HIGHT,TFT_BLACK);
     tft.setCursor (20,MENU_REC_HIGHT*i + 55);
     
     tft.print(Menu_Name[4*(page_num-1) + i]);
     tft.setCursor (240,MENU_REC_HIGHT*i + 55);
     tft.print(Food_price[4*(page_num-1) + i],DEC);
     
     tft.print("Rs");
  }  
  
  tft.fillRect(PRICE_BOX_X,PRICE_BOX_Y,PRICE_BOX_W,PRICE_BOX_H,TFT_YELLOW);
  tft.drawRect(PRICE_BOX_X,PRICE_BOX_Y,PRICE_BOX_W,PRICE_BOX_H,TFT_RED);
  
  tft.fillRect(ORDER_X,ORDER_Y,ORDER_W,ORDER_H,TFT_YELLOW);
  tft.setCursor(ORDER_X+5,ORDER_Y+10);
  tft.print("ODR");
  
  tft.fillRect(CANCEL_X,CANCEL_Y,CANCEL_W,CANCEL_H,TFT_YELLOW);
  tft.setCursor(CANCEL_X+5,CANCEL_Y+10);
  tft.print("CNL");

  tft.fillRect(FINAL_X,FINAL_Y,FINAL_W,FINAL_H,TFT_YELLOW);
  tft.setCursor(FINAL_X+20,FINAL_Y+5);
  tft.print("FINAL");
  
  tft.fillRect(PLUSE_X,PLUSE_Y,PLUSE_W,PLUSE_H,TFT_YELLOW);
  tft.setCursor(PLUSE_X+25,PLUSE_Y+10);
  tft.print("+");

  tft.fillRect(MINUS_X,MINUS_Y,MINUS_W,MINUS_H,TFT_YELLOW);
  tft.setCursor(MINUS_X+25,MINUS_Y+10);
  tft.print("-");

  tft.fillRect(PAGEUP_X,PAGEUP_Y,PAGEUP_W,PAGEUP_H,TFT_YELLOW);
  tft.setCursor(PAGEUP_X,PAGEUP_Y+5);
  tft.print("PREV");
  
  tft.fillRect(PAGEDOWN_X,PAGEDOWN_Y,PAGEDOWN_W,PAGEDOWN_H,TFT_YELLOW);
  tft.setCursor(PAGEDOWN_X,PAGEDOWN_Y+5);
  tft.print("NEXT");

  tft.drawLine(0,40,MENU_SQU,40,TFT_BLACK);
  tft.drawLine(MENU_SQU,0,MENU_SQU,VRES,TFT_BLACK);
}
