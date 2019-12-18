//!!AUM!!

#define EMG_PIN   A1
char LED[4] = {2,3,4,5};

#define HIGH_     0
#define LOW_     1

int arrayIndex = 0;
const int numOfReadings = 20;
int fifo_readings[numOfReadings];
unsigned int fifo_total = 0;

void setup() 
{ Serial.begin(9600);
  for(char i = 0;i<4;i++)
  { pinMode(LED[i],OUTPUT);
    digitalWrite(LED[i],LOW_);
  }
  delay(2000);
  for(char i = 0;i<3;i++)
  { for(char j = 0;j < 4;j++)
    { digitalWrite(LED[j],HIGH_);
      delay(250);
      digitalWrite(LED[j],LOW_);
    }
  }
}


int EMG_Signal = 0;
const int EmgRange[4] = {550,600,650,700}; 
void loop() 
{ int adc_row_value = analogRead(EMG_PIN);

  fifo_total = fifo_total - fifo_readings[arrayIndex];  
  fifo_readings[arrayIndex] = adc_row_value;
  fifo_total = fifo_total + fifo_readings[arrayIndex];
  EMG_Signal = fifo_total / numOfReadings;
  arrayIndex = arrayIndex + 1;
  if (arrayIndex >= numOfReadings)        arrayIndex = 0;

  for(char i = 0; i < 4; i++)
  { if(EMG_Signal > EmgRange[i])    digitalWrite(LED[i],HIGH_);
    else                            digitalWrite(LED[i],LOW_);
  }

  Serial.println(EMG_Signal);
  delay(20);
}
