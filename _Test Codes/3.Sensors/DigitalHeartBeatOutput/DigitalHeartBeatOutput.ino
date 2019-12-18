//!AUM!!

int delay1[15] = {7,6,8,5,7,8,4,5,9,10,8,7,6,8,10};
int delay2[15] = {77,75,78,80,82,75,79,81,85,76,77,80,74,78,79};
int count = 0;

void setup() 
{  Serial.begin(9600);
}


void loop() 
{ int i = 0;
  for(i = 0;i<delay1[count];i++)
  { 
  Serial.println(5,DEC);
  delay(1);
  }
  for(i = 0;i<delay2[count];i++)
  { 
  Serial.println(0,DEC);
  delay(1);
  }
  count++;
  if(count >= 15)  count = 0;
}

