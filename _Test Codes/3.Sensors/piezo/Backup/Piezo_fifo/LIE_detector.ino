//!!OM!!
#define PIEZO_SENSOR  A5

#define KEY1  4
#define KEY2  5
#define KEY3  6
#define KEY4  7

char Sample_No = 0;
unsigned char Sample_count = 0;
char Sample_Array_0[20];
char Sample_Array_1[20];
char Sample_Array_2[20];
char Sample_Array_3[20];
char Sample_Array_4[20];


void setup()
{ char i = 0;
  pinMode(PIEZO_SENSOR, INPUT);

  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  pinMode(KEY3, INPUT_PULLUP);
  pinMode(KEY4, INPUT_PULLUP);

  for(i = 0;i<20;i++)
  { Sample_Array_1[i] = 20;  
    Sample_Array_2[i] = 20; 
    Sample_Array_3[i] = 20; 
    Sample_Array_4[i] = 20; 
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
{
  if(digitalRead(KEY1) == 0)  Sample_No = 1;
  if(digitalRead(KEY2) == 0)  Sample_No = 2;
  if(digitalRead(KEY3) == 0)  Sample_No = 3;
  if(digitalRead(KEY4) == 0)  Sample_No = 4;    
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
  range_graph(range);
  
  Take_sample(range);

  for(i = 19; i > 0; i--)        //current sample fifo
  Sample_Array_0[i] = Sample_Array_0[i-1];
  Sample_Array_0[0] = range;

  Compare_Signal(); 
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

void Compare_Signal(void)
{ char i = 0,Sample_matched[4] = {0,0,0,0};
 
  for(i = 0;i<20;i++)
  { if(Sample_Array_0[i] == Sample_Array_1[i])
      Sample_matched[0]++;
    if(Sample_Array_0[i] == Sample_Array_2[i])
      Sample_matched[1]++;
    if(Sample_Array_0[i] == Sample_Array_3[i])
      Sample_matched[2]++;
    if(Sample_Array_0[i] == Sample_Array_4[i])
      Sample_matched[3]++;
  }

  if(Sample_matched[0] > 15)   
  { Serial.print("1 Sample Matched = ");
    Serial.println(Sample_matched[0]);
    delay(1000);
  } 
  if(Sample_matched[1] > 15)   
  { Serial.print("2 Sample Matched = ");
    Serial.println(Sample_matched[1]);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  if(Sample_matched[2] > 15)   
  { Serial.print("3 Sample Matched = ");
    Serial.println(Sample_matched[2]);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  if(Sample_matched[3] > 15)   
  { Serial.print("4 Sample Matched = ");
    Serial.println(Sample_matched[3]);
    delay(1000);
    Serial.println("\n\n\n\r");
  } 
  
}

void Take_sample(int sample)
{ if(Sample_No == 0) return;
  char i = 0;
  
  if(Sample_No == 1)  Sample_Array_1[Sample_count] = sample;
  if(Sample_No == 2)  Sample_Array_2[Sample_count] = sample;
  if(Sample_No == 3)  Sample_Array_3[Sample_count] = sample;
  if(Sample_No == 4)  Sample_Array_4[Sample_count] = sample;

  Sample_count++;
  if(Sample_count < 20)  return;
  Sample_count = 0;
  Sample_No = 0;    
  Serial.print("Sample Taken ");
  Serial.println(Sample_No);
  for(i = 0; i< 20;i++) range_graph(range);
  Serial.println("\n\n\n\r");
  
  delay(1000);
}



