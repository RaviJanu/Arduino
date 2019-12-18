//!!AUM!!



#define ACC_X_AXIS  A0
#define ACC_Y_AXIS  A1
#define ACC_Z_AXIS  A2
#define RIGHT_KEY   6
#define LEFT_KEY    7

#define BUFF_MAX      50

int Adc_buff[3][BUFF_MAX],prev_value[3];

void setup() 
{// put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RIGHT_KEY,INPUT_PULLUP);
  pinMode(LEFT_KEY,INPUT_PULLUP);
}

int base_ms = 0;

void loop() 
{ int x_axis = 0, y_axis = 0, z_axis = 0,Right = 0,Left = 0;
  int i = 0; 
  long ADC_Avg[3];
  
  analogReference(INTERNAL2V56);
  
  x_axis = analogRead(ACC_X_AXIS);
  y_axis = analogRead(ACC_Y_AXIS);
  z_axis = analogRead(ACC_Z_AXIS);
  Right  = digitalRead(RIGHT_KEY);
  Left   = digitalRead(LEFT_KEY);  
  
  for(i = 0; i < BUFF_MAX-1 ; i++)
  {   Adc_buff[0][i] = Adc_buff[0][i+1];
      ADC_Avg[0] += Adc_buff[0][i]; 
      Adc_buff[1][i] = Adc_buff[1][i+1];
      ADC_Avg[1] += Adc_buff[1][i]; 
      Adc_buff[2][i] = Adc_buff[2][i+1];
      ADC_Avg[2] += Adc_buff[2][i]; 
  }
  
  Adc_buff[0][BUFF_MAX-1] = x_axis;
  ADC_Avg[0] += Adc_buff[0][BUFF_MAX-1];
  ADC_Avg[0] /= BUFF_MAX;
  Adc_buff[1][BUFF_MAX-1] = y_axis;
  ADC_Avg[1] += Adc_buff[1][BUFF_MAX-1];
  ADC_Avg[1] /= BUFF_MAX;
  Adc_buff[2][BUFF_MAX-1] = z_axis;
  ADC_Avg[2] += Adc_buff[2][BUFF_MAX-1];
  ADC_Avg[2] /= BUFF_MAX;

  for(i = 0; i < 3 ; i++)
  {   if(ADC_Avg[i] > 600) 
      { ADC_Avg[i] = ADC_Avg[i] - 600;
        if((prev_value[i] > (ADC_Avg[i] + 3))||(prev_value[i] < (ADC_Avg[i] - 3)))
        { if(ADC_Avg[i] >= prev_value[i]) prev_value[i]++;
          else prev_value[i]--;
        }
      }
      else                  ADC_Avg[i] = 0;
  }

  delay(1);
  base_ms++;
  if(base_ms < 100) return; 
  base_ms = 0;

  Serial.print(String(prev_value[0])+","+String(prev_value[1])+","+String(prev_value[2])+","+String(Right)+","+String(Left)+"__");
}
