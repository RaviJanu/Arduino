#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);
#define HALL_SENSOR_1     A0
#define HALL_SENSOR_2     A1
#define HALL_SENSOR_3     A2
#define HALL_SENSOR_4     A3

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
unsigned long Command = 1;
void setup()
{   Serial.begin(9600);
    pinMode(HALL_SENSOR_1,INPUT);
    pinMode(HALL_SENSOR_2,INPUT);
    pinMode(HALL_SENSOR_3,INPUT);
    pinMode(HALL_SENSOR_4,INPUT);

    radio.begin();
    radio.setRetries(15,15);
    radio.openReadingPipe(1,pipes[1]);
    radio.startListening();
    radio.printDetails();
    radio.openWritingPipe(pipes[0]); 
    radio.openReadingPipe(1,pipes[1]);
    radio.stopListening();
}

void loop(void)
{ char Hall_1,Hall_2,Hall_3,Hall_4,r = 0;
  byte Hall_Sensor = 0;
//  radio.stopListening();
  
  Hall_1 = digitalRead(HALL_SENSOR_1);
  Hall_2 = digitalRead(HALL_SENSOR_2);
  Hall_3 = digitalRead(HALL_SENSOR_3);
  Hall_4 = digitalRead(HALL_SENSOR_4);
  Hall_Sensor |= (Hall_1 & 0x01) | (Hall_2 << 1) | (Hall_3 << 2) | (Hall_4 << 3);

  if(Hall_Sensor == 0x0E)
  { r = 'Q'; 
  }
  if(Hall_Sensor == 0x0D)
  { r = 'W'; 
  }  
  if(Hall_Sensor == 0x0B)
  { r = 'E'; 
  }  
  if(Hall_Sensor == 0x07)
  { r = 'R'; 
  }
  if(Hall_Sensor == 0x0F)
  { r = 'X'; 
  }
//  radio.write(&r,sizeof(char));
//  Serial.print(Hall_Sensor,HEX);
    Serial.print(r);
    
//    radio.startListening();
    delay(500);
    
}
