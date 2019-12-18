//!AUM!!


void setup() 
{  Serial.begin(9600);
}

void loop() 
{ int analog_value1 = 0,analog_value2 = 0,cmpar_out = 0;

  analog_value1 = analogRead(A0);
  analog_value2 = analogRead(A1);
  cmpar_out = analog_value1 - analog_value2;
  Serial.println(cmpar_out,DEC);
  delay(20);
}



