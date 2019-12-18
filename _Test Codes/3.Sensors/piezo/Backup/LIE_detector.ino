
//!!OM!!
#include <LiquidCrystal.h>
// RS-12  ; EN-11 ; D4-5    ;  D5-4   ; D6-3    ; D7-2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


#define PIEZO_SENSOR  A5

void setup()
{
  pinMode(PIEZO_SENSOR, INPUT);

  Serial.begin(115200);
  Serial.print("LIE Detector");

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("LIE Detector");
}

unsigned int pulse_delay_ms = 0, lie_indication_time = 0;

void loop()
{ delay(20);
  arduinoSerialMonitorVisual();
}

//  Code to Make the Serial Monitor Visualizer Work
void arduinoSerialMonitorVisual(void)
{
  int ADC_Count = 0;
  ADC_Count = analogRead(PIEZO_SENSOR);

  // map the sensor range to a range of 12 options:
  int range = map(ADC_Count, 200,600, 0, 11);

  Serial.print(ADC_Count); 
  // do something different depending on the
  // range value:
  switch (range) {
    case 0:
      Serial.println("");     /////ASCII Art Madness
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
      Serial.println("--------------|-----------------------");
      break;

  }


}





