//!!OM!!
#include <LiquidCrystal.h>
// RS-7  ; EN-6 ; D4-5    ;  D5-4   ; D6-3    ; D7-2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define PULSESENSOR_DIG 8



void setup() 
{// put your setup code here, to run once:
  pinMode(PULSESENSOR_DIG,INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("HeartBeat Sense ");
}

void loop() 
{// put your main code here, to run repeatedly:
    Dig_Pulse_Sence();
}



int pulse_delay_count = 0;
char beat_detect_flag = 0;

void Dig_Pulse_Sence(void)
{ int pulse_rate,dig_pulse_val = 0;
  dig_pulse_val = digitalRead(PULSESENSOR_DIG);
  Serial.println(dig_pulse_val,DEC);  
  if(dig_pulse_val == 1)
  { if(beat_detect_flag == 0)
    { beat_detect_flag = 1;
      pulse_rate = 60000 / (pulse_delay_count*20);
      if((pulse_rate > 60)&&(pulse_rate < 120))
      { lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(pulse_rate,DEC);
        lcd.print(" bpm");
      }
      pulse_delay_count = 0;
    }
    lcd.setCursor(15, 1);
    lcd.print("*");
  }
  else
  {  lcd.setCursor(15, 1);
     lcd.print(" ");
     beat_detect_flag = 0;
  }  
  pulse_delay_count++; 
  if(pulse_delay_count > 60)  pulse_delay_count = 0;

  delay(20);
}






