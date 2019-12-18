//!!AUM!!

#define MAX_LAN    4
#define MAX_IR     3
byte IR_SENSR[MAX_LAN][MAX_IR] = 
{ {42,43,44},
  {45,46,47},
  {48,49,50},
  {51,52,53}
};

#define   MAX_LED   5
#define   ONN       1       
#define   OFF       0
#define   RED         0
#define   YELLOW      1
#define   MGREEN      2
#define   LGREEN      3
#define   RGREEN      4
byte LED_ARRAY[MAX_LAN][MAX_LED] = 
{ {22,23,24,25,26},
  {27,28,29,30,31},
  {32,33,34,35,36},
  {37,38,39,40,41},
};

byte LanDelay[] = {0,0,0,0};
byte IR_STATUS_ARRAY[MAX_LAN] = {0,0,0,0};
byte PresentLan = 0,Start_stop = 0;


void setup() 
{ byte i = 0,j = 0;
  for(i = 0;i < MAX_LAN;i++)
  { for(j = 0;j < MAX_IR;j++)     pinMode(IR_SENSR[i][j],INPUT);   //pinMode(IR_SENSR[i][j],INPUT);
    for(j = 0;j < MAX_LED;j++)    pinMode(LED_ARRAY[i][j],OUTPUT);
  }
  ALL_LED_OFF();
  delay(2000);
  Serial.begin(9600);
}


void ALL_LED_OFF(void)
{ byte i = 0,j = 0;
  for(i = 0;i < MAX_LAN;i++)
  { for(j = 0;j < MAX_LED;j++)    digitalWrite(LED_ARRAY[i][j],HIGH);
  }
}

unsigned int base_ms = 0;
void loop() 
{ if(Serial.available())
  { char r = Serial.read();
    if(r == 'Q')  Start_stop = 1;
    if(r == 'W')  Start_stop = 0;
  }
  delay(1);
  base_ms++;
  if(base_ms < 1000)    return;
  base_ms = 0;
  Traffic_Logic();
}


char yellow_blink_flg = 0;
void Traffic_Logic(void)
{ byte i = 0,j = 0,similer_lan = 1;
  
  if(Start_stop == 0)   
  { if(yellow_blink_flg == 0)
    {   yellow_blink_flg = 1;  
        for(j = 0;j < MAX_LAN;j++)  digitalWrite(LED_ARRAY[j][YELLOW],LOW);
    }
    else
    {   yellow_blink_flg  = 0;
        for(j = 0;j < MAX_LAN;j++)  digitalWrite(LED_ARRAY[j][YELLOW],HIGH);
    }
    return;
  }
  
  LanLogic_ScanePrint();
  
  ALL_LED_OFF();
  for(char i = 0;i<MAX_LAN;i++)
  { if(i == PresentLan)
    {   digitalWrite(LED_ARRAY[PresentLan][LGREEN],LOW);
        digitalWrite(LED_ARRAY[PresentLan][MGREEN],LOW);
        digitalWrite(LED_ARRAY[PresentLan][RGREEN],LOW);
    }
    else
    {   digitalWrite(LED_ARRAY[i][RED],LOW);
    }
  }
//  delay(LanDelay[PresentLan]*100);     
  ALL_LED_OFF();
  digitalWrite(LED_ARRAY[PresentLan][YELLOW],LOW);
//  delay(3000);
  ALL_LED_OFF();
    

  PresentLan++;
  if(PresentLan >= MAX_LAN)  
  { PresentLan = 0;
  }
}



void LanLogic_ScanePrint(void)
{// Serial.print(PresentLan);             //for   testing
  if(PresentLan != 0)   return;
  char i =0,j = 0;
  byte car_count = 0,similer_lan = 1;
   byte rotate[4] = {1,2,3,0}; 
  
  for(i=0;i<MAX_LAN;i++)
  {   car_count = 0;
      for(j=0;j<MAX_IR;j++)
      {  if(digitalRead(IR_SENSR[i][j]) == 1)     car_count++;
      }
      IR_STATUS_ARRAY[i] = car_count;
   }
   for(i=0;i<MAX_LAN;i++)
   {  if(IR_STATUS_ARRAY[i] == IR_STATUS_ARRAY[rotate[i]])
      { similer_lan++;
      }
   }
   if(similer_lan > 4)  similer_lan = 4;
   
   if(similer_lan == 1)    NO_LAN_SIMILER();
   if(similer_lan == 3)    THREE_LAN_SIMILER();
   if(similer_lan == 4)    ALL_LAN_SIMILER();

   for(char i = 0;i<MAX_LAN;i++) 
   {  Serial.write('A'+i);
      Serial.print(IR_STATUS_ARRAY[i]);
   }
   for(char i = 0;i<MAX_LAN;i++) 
   {  Serial.write('E'+i);
      Serial.print(LanDelay[i]);
   }
   Serial.println("_");
}

 
void ALL_LAN_SIMILER(void)
{ LanDelay[0] = LanDelay[1] = LanDelay[2] = LanDelay[3] = 30;
}

void THREE_LAN_SIMILER(void)
{/* byte DelayArray[4][3][2] = 
  { {{20,30},{15,35},{10,35}},      //lan value 0
    {{40,25},{25,30},{15,35}},      //lan value 1
    {{75,15},{60,20},{25,30}},      //lan value 2
    {{90,10},{60,20},{45,25}},      //lan value 3
  };
*/
  byte DelayArray[12][2] = 
  { {20,30},{15,35},{10,35},      //lan value 0
    {40,25},{25,30},{15,35},      //lan value 1
    {75,15},{60,20},{25,30},      //lan value 2
    {90,10},{60,20},{45,25},      //lan value 3
  };


   byte i = 0,j =0,diff_value[2] = {0,0},diff_lane = 3,rotate[4] = {1,2,3,0}; 

   for(i=0;i<MAX_LAN;i++)
   {  byte count = 0;
      for(j=0;j<MAX_LAN;j++)
      { if(IR_STATUS_ARRAY[i] != IR_STATUS_ARRAY[rotate[j]])    count++;
      }
      if(count == 3)    
      { diff_lane = i;
        break;
      }
   }
   
   
   for(i=0;i<12;i++)
   {  if(i == diff_lane)
      { LanDelay[IR_STATUS_ARRAY[i/3]] = DelayArray[i][0];
      }
      else
      { LanDelay[IR_STATUS_ARRAY[i/3]] = DelayArray[i][1];
      }
   }
/*
   for(i=0;i<MAX_LAN;i++)
   {  if(i == diff_lane)
      { if(diff_value[0] == 0)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][0][1];
        if(diff_value[0] == 1)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][1][1];
        if(diff_value[0] == 2)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][2][1];
        if(diff_value[0] == 3)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][3][1];
      }
      else
      { if(diff_value[1] == 0)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][i][0];
        if(diff_value[1] == 1)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][i][0];
        if(diff_value[1] == 2)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][i][0];
        if(diff_value[1] == 3)  LanDelay[IR_STATUS_ARRAY[i]] = DelayArray[IR_STATUS_ARRAY[i]][i][0];
      }
   }
*/
   Serial.print('_');
   Serial.print(diff_lane);
   Serial.print('_');

   
/*   for(i=0;i<MAX_LAN;i++)
   {  if(i == diff_lane)
      { if(diff_value[0] == 0)  LanDelay[i] = DelayArray[0][i][0];
        if(diff_value[0] == 1)  LanDelay[i] = DelayArray[1][i][0];
        if(diff_value[0] == 2)  LanDelay[i] = DelayArray[2][i][0];
        if(diff_value[0] == 3)  LanDelay[i] = DelayArray[3][i][0];
      }
      else
      { if(diff_value[1] == 0)  LanDelay[i] = DelayArray[0][i][1];
        if(diff_value[1] == 1)  LanDelay[i] = DelayArray[1][i][1];
        if(diff_value[1] == 2)  LanDelay[i] = DelayArray[2][i][1];
        if(diff_value[1] == 3)  LanDelay[i] = DelayArray[3][i][1];
      }
   }
 */
}

void NO_LAN_SIMILER(void)
{ byte DelayArray[] = {15,25,35,45};
  for(char i = 0;i<MAX_LAN;i++)
  { LanDelay[i] = DelayArray[IR_STATUS_ARRAY[i]];  
  }
}



