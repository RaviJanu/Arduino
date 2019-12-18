//!!OM!!
#define TEMPSENSOR_PIN  A0
#define PULSESENSOR_ANG A2
#define PIEZO_PIN  A5
#define PULSESENSOR_DIG 7

unsigned char pulse_cntr = 0;

void setup() 
{// put your setup code here, to run once:
  pinMode(TEMPSENSOR_PIN,INPUT);
  pinMode(PULSESENSOR_ANG,INPUT);
  pinMode(PULSESENSOR_DIG,INPUT);
  pinMode(PIEZO_PIN,INPUT);
  Serial.begin(115200);
}

void loop() 
{// put your main code here, to run repeatedly:
//    Temp_Sence();
    Dig_Pulse_Sence();
//      piezo_sensor();
}


void piezo_sensor(void)
{
  int ADC_Count = 0;
  ADC_Count = analogRead(PIEZO_PIN);  
  Serial.println(ADC_Count,DEC); 

  delay(100);
}


int temp_loopdelay = 0;
void Temp_Sence(void)
{ temp_loopdelay++;
  if(temp_loopdelay < 1000) return;
  temp_loopdelay = 0;

  int ADC_Count = 0;
  float Celsuis = 0;
  ADC_Count = analogRead(TEMPSENSOR_PIN);  
//  Serial.print("ADC count :  ");
//  Serial.print(ADC_Count,DEC);
  Celsuis = (ADC_Count*500)/1023;
  Serial.print("Temp : ");
  Serial.println((int)Celsuis,DEC);
}

void Anlg_Pulse_Sence(void)
{ int PulseADC_Count = 0;

  PulseADC_Count = analogRead(PULSESENSOR_ANG);  
  Serial.print(PulseADC_Count,DEC);
  Serial.print("  ");
  pulse_cntr++;
  if(pulse_cntr > 72)
  { pulse_cntr = 0;
    Serial.println("/////\n\n\n\r");
    delay(5000);
  }
  delay(13); 
}

int pulse_counter = 0, one_min_count = 0,mil_min_counter = 0;
char flag = 0;


void Dig_Pulse_Sence(void)
{ 
    if(digitalRead(PULSESENSOR_DIG) == 1)
    { if(flag == 0)
      { flag = 1;
        one_min_count++;
        Serial.println(pulse_counter,DEC);
        pulse_counter = 0;
      }
    }
    else flag = 0;

    pulse_counter++;
    delay(1);

    mil_min_counter++;
    if(mil_min_counter > 5*1000)
    {   mil_min_counter = 0;
        Serial.print("30 sec : ");
        Serial.println(one_min_count,DEC);  
        one_min_count = 0;
 //     delay(3000);
    }
}






