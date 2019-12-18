//!!OM!!
#define PIEZO_SENSOR  A5

#define KEY1  4
#define KEY2  5
#define KEY3  6
#define KEY4  7

#define  MAX_SAMPLE        50
#define  NUM_RESULOTION    30 
#define  AMP_RESULOTION    2

char Sample_No = 0;
char Sample_count = 0;
unsigned int Sample_Array[5][MAX_SAMPLE];
char ota_flag = 0;


void setup()
{ char i = 0;
  pinMode(PIEZO_SENSOR, INPUT);

  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  pinMode(KEY3, INPUT_PULLUP);
  pinMode(KEY4, INPUT_PULLUP);

  for(i = 0;i<MAX_SAMPLE;i++)
  { Sample_Array[1][i] = 20;  
    Sample_Array[2][i] = 20; 
    Sample_Array[3][i] = 20; 
    Sample_Array[4][i] = 20; 
  }

  Serial.begin(115200);
  Serial.print("LIE Detector");
  
}

unsigned int pulse_delay_ms = 0, lie_indication_time = 0;

void loop()
{ delay(20);
  arduinoSerialMonitorVisual();
  key_sence();
}

void key_sence(void)
{ if(digitalRead(KEY1) == 0)   Sample_No = 1;     
  if(digitalRead(KEY2) == 0)   Sample_No = 2;    
if(digitalRead(KEY3) == 0)     Sample_No = 3;    
  if(digitalRead(KEY4) == 0)   Sample_No = 4;    
}


//  Code to Make the Serial Monitor Visualizer Work
void arduinoSerialMonitorVisual(void)
{ int range = 0,i = 0;
  int ADC_Count = 0;
  
  ADC_Count = analogRead(PIEZO_SENSOR);
  // map the sensor range to a range of 12 options:
  if((ADC_Count >= 200)&&(ADC_Count <= 600))
  range = map(ADC_Count, 200,600, 1, 10);
  if(ADC_Count > 600) range = 11;

  Serial.print(ADC_Count); 
  // do something different depending on the
  // range value:
  for(i = MAX_SAMPLE-1; i > 0; i--)        //current sample fifo
  Sample_Array[0][i] = Sample_Array[0][i-1];
  Sample_Array[0][0] = range;

  range_graph(range);    
  Take_sample(range);
  Compare_Signal(); 
}

void Take_sample(int sample)
{ if(Sample_No == 0) return;

  char i = 0,Sample_matched = 0;

  if(ota_flag == 0)   //one time avoid flag
  { if ( ((Sample_Array[0][0]-Sample_Array[0][1])<2) && ((Sample_Array[0][0]-Sample_Array[0][2])<2) )      return;
    ota_flag = 1;
  }

  for(i = Sample_count; i > 0; i--)        //current sample fifo
  Sample_Array[Sample_No][i] = Sample_Array[Sample_No][i-1];
  Sample_Array[Sample_No][0] = sample;

//  Sample_Array[Sample_No][Sample_count] = sample;

  
  Sample_count++;
  if(Sample_count < MAX_SAMPLE)  return;

  Serial.print("Sample Taken ");
  Serial.println(Sample_No,DEC);
  for(i = 0; i< MAX_SAMPLE;i++) range_graph(Sample_Array[Sample_No][i]);
  Serial.println("current fifo");
  for(i = 0; i< MAX_SAMPLE;i++) range_graph(Sample_Array[0][i]);
  
  for(i = 0;i<MAX_SAMPLE;i++)
  { if(Sample_Array[0][i] == Sample_Array[Sample_No][i])
      Sample_matched++;
  }
  Serial.println(Sample_matched,DEC);
  Serial.println("\n\n\r");  

  Sample_count = 0;
  Sample_No = 0;
  ota_flag = 0;
  
  delay(1000);
}


void Compare_Signal(void)
{ char i = 0,Sample_matched[4] = {0,0,0,0};
  
  if(ota_flag == 1) return;

 /*
  for(i = 0;i<MAX_SAMPLE;i++)
  { if(Sample_Array[0][i] == Sample_Array[1][i])
      Sample_matched[0]++;
    if(Sample_Array[0][i] == Sample_Array[2][i])
      Sample_matched[1]++;
    if(Sample_Array[0][i] == Sample_Array[3][i])
      Sample_matched[2]++;
    if(Sample_Array[0][i] == Sample_Array[4][i])
      Sample_matched[3]++;
  }
*/

  for(i = 0;i<MAX_SAMPLE;i++)
  { if((Sample_Array[0][i] - Sample_Array[1][i]) < AMP_RESULOTION )
      Sample_matched[0]++;
    if((Sample_Array[0][i] - Sample_Array[2][i]) < AMP_RESULOTION)
      Sample_matched[1]++;
    if((Sample_Array[0][i] - Sample_Array[3][i]) < AMP_RESULOTION)
      Sample_matched[2]++;
    if((Sample_Array[0][i] - Sample_Array[4][i]) < AMP_RESULOTION)
      Sample_matched[3]++;
  }


  if(Sample_matched[0] > NUM_RESULOTION)   
  { Serial.print("1 Sample Matched = ");
    Serial.println(Sample_matched[0],DEC);
    delay(1000);
  } 
  if(Sample_matched[1] > NUM_RESULOTION)   
  { Serial.print("2 Sample Matched = ");
    Serial.print(Sample_matched[1],DEC);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  if(Sample_matched[2] > NUM_RESULOTION)   
  { Serial.print("3 Sample Matched = ");
    Serial.println(Sample_matched[2],DEC);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  if(Sample_matched[3] > NUM_RESULOTION)   
  { Serial.print("4 Sample Matched = ");
    Serial.println(Sample_matched[3],DEC);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  
}


void  range_graph(char value)
{
  switch (value) 
  {
     case 0:
      Serial.println("| ");
      break;
    case 1:
      Serial.println("---");
      break;
    case 2:
      Serial.println("------");
      break;
    case 3:
      Serial.println("---------");
      break;
    case 4:
      Serial.println("------------");
      break;
    case 5:
      Serial.println("--------------|-");
      break;
    case 6:
      Serial.println("--------------|---");
      break;
    case 7:
      Serial.println("--------------|-------");
      break;
    case 8:
      Serial.println("--------------|----------");
      break;
    case 9:
      Serial.println("--------------|----------------");
      break;
    case 10:
      Serial.println("--------------|-------------------");
      break;
    case 11:
      Serial.println("--------------|-----------------------|");
      break;
  }
}



/*  for(i =MAX_SAMPLE-1; i > 0; i--)        //current sample fifo
  Sample_Array[0][i] = Sample_Array[0][i-1];
  Sample_Array[0][0] = range;
 * */
 


/*
 * 
  if(ota_flag == 0)   //one time avoid flag
  { for(i =  0; i < 3;i++)
    { if ( ((Sample_Array[0][0]-Sample_Array[0][i])<2) && ((Sample_Array[0][0]-Sample_Array[0][i]) > -2) )      
       return;
    }
    ota_flag = 1;
  }

    Sample_Array[Sample_No][Sample_count] = sample;
 */
//  for(i =MAX_SAMPLE; i>0;i--) range_graph(Sample_Array[Sample_No][i]);

