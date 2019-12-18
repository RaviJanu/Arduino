//!AUM!!

#define BUFF_MAX      100
int analog_value1 = 0,analog_value2 = 0,cmpar_out = 0;
int Adc_buff[BUFF_MAX];

void setup() 
{  Serial.begin(9600);
}

void loop() 
{ int i = 0; 
  long ADC_Avg = 0;
  analog_value1 = analogRead(A0);

  for(i = 0; i < BUFF_MAX-1 ; i++)
  {   Adc_buff[i] = Adc_buff[i+1];
      ADC_Avg += Adc_buff[i]; 
  }
  Adc_buff[BUFF_MAX-1] = analog_value1;
  ADC_Avg += Adc_buff[BUFF_MAX-1];
  ADC_Avg /= BUFF_MAX;
  
  Serial.println(ADC_Avg,DEC);
  delay(10);
}



