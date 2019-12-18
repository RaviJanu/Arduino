 //!!AUM!!
//INIT...

#define LEFT_MOTOR_1        2
#define LEFT_MOTOR_2        3
#define RIGHT_MOTOR_1       4
#define RIGHT_MOTOR_2       5

#define FRONT_LEFT_IR       8
#define FRONT_RIGHT_IR      9

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

    
  pinMode(FRONT_RIGHT_IR,INPUT);
  pinMode(FRONT_LEFT_IR,INPUT);

  Serial.begin(9600);
  delay(5000);
}

unsigned int base_ms = 0,duration_1  = 0;

void loop() 
{ char FrontCenterIR = 0,FrontRightIR = 0,FrontLeftIR = 0;
  
  FrontRightIR = digitalRead(FRONT_RIGHT_IR);
  FrontLeftIR = digitalRead(FRONT_LEFT_IR);
  

     if((FrontLeftIR == 0)&&(FrontRightIR == 0))        //Forward Action
     {   ROBOT_MOVMENT(MOV_FORWARD);
     }
     if((FrontLeftIR == 0)&&(FrontRightIR == 1))        //Left Action
     {   ROBOT_MOVMENT(MOV_LEFT);
     }
     if((FrontLeftIR == 1)&&(FrontRightIR == 0))        //Right Action
     {   ROBOT_MOVMENT(MOV_RIGHT);
     }
     
     if((FrontLeftIR == 1)&&(FrontRightIR == 1))        //Stop Action
     {   ROBOT_MOVMENT(MOV_STOP);
     }
    delay(10);

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
