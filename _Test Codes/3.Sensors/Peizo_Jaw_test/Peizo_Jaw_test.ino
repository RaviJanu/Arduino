//!AUM!!

void setup() 
{  Serial.begin(9600);
}

void loop() 
{ int analog_value = 0, map_value = 0;
  analog_value = analogRead(A0);
  map_value = map(analog_value,0,1024,0,50);
  Serial.println(map_value);
  delay(20);
}














