//!AUM!!


void setup() 
{  Serial.begin(9600);
}

void loop() 
{ int analog_value = 0;

  analog_value = analogRead(A0);
  Serial.println(analog_value,DEC);
  delay(20);
}



