 //!!AUM!!
//INIT...

#define LEFT_MOTOR_1        12
#define LEFT_MOTOR_2        11
#define RIGHT_MOTOR_1       10
#define RIGHT_MOTOR_2       9

#define SOUND_SENSOR        A0

#define MOV_FORWARD       0
#define MOV_BACKWARD      1
#define MOV_LEFT          2
#define MOV_RIGHT         3
#define MOV_STOP          4  

void setup() 
{  // put your setup code here, to run once:
  pinMode(LEFT_MOTOR_1,OUTPUT);
  pinMode(LEFT_MOTOR_2,OUTPUT);
  pinMode(RIGHT_MOTOR_1,OUTPUT);
  pinMode(RIGHT_MOTOR_2,OUTPUT);

  digitalWrite(LEFT_MOTOR_1,LOW);
  digitalWrite(LEFT_MOTOR_2,LOW);
  digitalWrite(RIGHT_MOTOR_1,LOW);
  digitalWrite(RIGHT_MOTOR_2,LOW);

    
  Serial.begin(9600);
  delay(5000);
}


void loop() 
{   unsigned int clap_detect = 0;

    clap_detect = analogRead(SOUND_SENSOR);
    Serial.println(clap_detect);
    
    if(clap_detect < 500)    ROBOT_MOVMENT(MOV_FORWARD);
    else
    {   ROBOT_MOVMENT(MOV_STOP);
        delay(1000);
        ROBOT_MOVMENT(MOV_BACKWARD);
        delay(1000);
        ROBOT_MOVMENT(MOV_STOP);
        delay(1000);
        ROBOT_MOVMENT(MOV_LEFT);
        delay(700);
        ROBOT_MOVMENT(MOV_STOP);
        delay(1000);
    }
    delay(20);
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










