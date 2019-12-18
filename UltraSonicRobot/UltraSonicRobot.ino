 //!!AUM!!
//INIT...

#define LEFT_MOTOR_1        12
#define LEFT_MOTOR_2        11
#define RIGHT_MOTOR_1       10
#define RIGHT_MOTOR_2       9

#define US_1_Echo           7
#define US_1_Trig           8
#define US_2_Echo           6
#define US_2_Trig           5

#define MOV_FORWARD       0
#define MOV_BACKWARD      1
#define MOV_LEFT          2
#define MOV_RIGHT         3
#define MOV_STOP          4  

#define DISTANCE          5


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

  pinMode(US_1_Trig, OUTPUT);
  pinMode(US_1_Echo, INPUT);
  pinMode(US_2_Trig, OUTPUT);
  pinMode(US_2_Echo, INPUT);
    
  Serial.begin(9600);
  delay(5000);
}


void loop() 
{   unsigned int base_ms = 0,duration_1  = 0,duration_2  = 0;

    digitalWrite(US_1_Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_1_Trig, LOW);
    duration_1 = microsecondsToInches(pulseIn(US_1_Echo, HIGH)); 
    digitalWrite(US_2_Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_2_Trig, LOW);
    duration_2 = microsecondsToInches(pulseIn(US_2_Echo, HIGH)); 
    
    if((duration_1 > DISTANCE )&&(duration_2 > DISTANCE ))    ROBOT_MOVMENT(MOV_FORWARD);
    if((duration_1 < DISTANCE )&&(duration_2 > DISTANCE ))    ROBOT_MOVMENT(MOV_LEFT);
    if((duration_1 > DISTANCE )&&(duration_2 < DISTANCE ))    ROBOT_MOVMENT(MOV_RIGHT);     
    if((duration_1 < DISTANCE )&&(duration_2 < DISTANCE ))   
    { ROBOT_MOVMENT(MOV_STOP);  
      delay(1000);
      ROBOT_MOVMENT(MOV_BACKWARD);  
      delay(1000);
      ROBOT_MOVMENT(MOV_STOP);  
      delay(1000);
      ROBOT_MOVMENT(MOV_LEFT);  
      delay(2000);
      ROBOT_MOVMENT(MOV_STOP);  
      delay(1000);
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


long microsecondsToInches(long microseconds)
{ // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}



















