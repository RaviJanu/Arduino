#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define LEFT_MOTOR_1      2
#define LEFT_MOTOR_2      3
#define RIGHT_MOTOR_1     4
#define RIGHT_MOTOR_2     5

#define MOV_STOP          'X'
#define MOV_FORWARD       'Q'
#define MOV_BACKWARD      'W'
#define MOV_LEFT          'E'
#define MOV_RIGHT         'R'

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
typedef enum { role_ping_out = 1, role_pong_back } role_e;
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};
role_e role = role_pong_back;
void setup(void)
{   pinMode(LEFT_MOTOR_1,OUTPUT);
    pinMode(LEFT_MOTOR_2,OUTPUT);
    pinMode(RIGHT_MOTOR_1,OUTPUT);
    pinMode(RIGHT_MOTOR_2,OUTPUT);
    ROBOT_MOVMENT(MOV_STOP);
  
    Serial.begin(9600);
    radio.begin();
    radio.setRetries(15,15);
    radio.openReadingPipe(1,pipes[1]);
    radio.startListening();
    radio.printDetails();
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
}

void loop(void)
{   if ( radio.available() )
    { char data = 0;
      radio.read( &data, sizeof(char) );
      ROBOT_MOVMENT(data);
      Serial.println(data);
    }
}

void ROBOT_MOVMENT(char MOVMENT)
{ if(MOVMENT == MOV_FORWARD)
  {   digitalWrite(LEFT_MOTOR_1,HIGH);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,HIGH);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
  if(MOVMENT == MOV_BACKWARD)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,HIGH);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_LEFT)
  {   digitalWrite(LEFT_MOTOR_1,HIGH);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_RIGHT)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,HIGH);
      digitalWrite(RIGHT_MOTOR_1,HIGH);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
  if(MOVMENT == MOV_STOP)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,LOW);
  }
}

