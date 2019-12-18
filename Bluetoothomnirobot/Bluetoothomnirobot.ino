

#define FORNT_MOTOR_1     2
#define FORNT_MOTOR_2     3
#define FORNT_MOTOR_3     4
#define FORNT_MOTOR_4     5
#define SIDE_MOTOR_1      6
#define SIDE_MOTOR_2      7
#define SIDE_MOTOR_3      8
#define SIDE_MOTOR_4      9 


#define MOV_STOP          0
#define MOV_FORWARD       1
#define MOV_BACKWARD      2
#define MOV_LEFT          3
#define MOV_RIGHT         4
#define MOV_CLK           5
#define MOV_ANTICLK       6

void setup() 
{ // put your setup code here, to run once:
  
  pinMode(FORNT_MOTOR_1,OUTPUT);
  pinMode(FORNT_MOTOR_2,OUTPUT);
  pinMode(FORNT_MOTOR_3,OUTPUT);
  pinMode(FORNT_MOTOR_4,OUTPUT);
  pinMode(SIDE_MOTOR_1,OUTPUT);
  pinMode(SIDE_MOTOR_2,OUTPUT);
  pinMode(SIDE_MOTOR_3,OUTPUT);
  pinMode(SIDE_MOTOR_4,OUTPUT);
  
  digitalWrite(FORNT_MOTOR_1,LOW);
  digitalWrite(FORNT_MOTOR_2,LOW);
  digitalWrite(FORNT_MOTOR_3,LOW);
  digitalWrite(FORNT_MOTOR_4,LOW);
  digitalWrite(SIDE_MOTOR_1,LOW);
  digitalWrite(SIDE_MOTOR_2,LOW);
  digitalWrite(SIDE_MOTOR_3,LOW);
  digitalWrite(SIDE_MOTOR_4,LOW);
  
  Serial.begin(9600);
}
void loop()
{   serialdata();
}

void serialdata()
{   if(!Serial.available())    return;
    char val = Serial.read();
    if (val == 'F') ROBOT_MOVMENT(MOV_FORWARD);
    if (val == 'B') ROBOT_MOVMENT(MOV_BACKWARD);
    if (val== 'L')  ROBOT_MOVMENT(MOV_LEFT);
    if (val == 'R') ROBOT_MOVMENT(MOV_RIGHT);
    if (val == 'C') ROBOT_MOVMENT(MOV_CLK);
    if (val == 'A') ROBOT_MOVMENT(MOV_ANTICLK);
    if (val == 'S') ROBOT_MOVMENT(MOV_STOP);
}

void ROBOT_MOVMENT(char MOVMENT)
{ if(MOVMENT == MOV_FORWARD)
  {   digitalWrite(FORNT_MOTOR_1,LOW);
      digitalWrite(FORNT_MOTOR_2,HIGH);
      digitalWrite(FORNT_MOTOR_3,LOW);
      digitalWrite(FORNT_MOTOR_4,HIGH);
      digitalWrite(SIDE_MOTOR_1,LOW);
      digitalWrite(SIDE_MOTOR_2,HIGH);
      digitalWrite(SIDE_MOTOR_3,LOW);
      digitalWrite(SIDE_MOTOR_4,HIGH);
  }
  if(MOVMENT == MOV_BACKWARD)
  {   digitalWrite(FORNT_MOTOR_1,HIGH);
      digitalWrite(FORNT_MOTOR_2,LOW);
      digitalWrite(FORNT_MOTOR_3,HIGH);
      digitalWrite(FORNT_MOTOR_4,LOW);
      digitalWrite(SIDE_MOTOR_1,HIGH);
      digitalWrite(SIDE_MOTOR_2,LOW);
      digitalWrite(SIDE_MOTOR_3,HIGH);
      digitalWrite(SIDE_MOTOR_4,LOW);
  }
  if(MOVMENT == MOV_LEFT)
  {   digitalWrite(FORNT_MOTOR_1,HIGH);
      digitalWrite(FORNT_MOTOR_2,LOW);
      digitalWrite(FORNT_MOTOR_3,LOW);
      digitalWrite(FORNT_MOTOR_4,HIGH);
      digitalWrite(SIDE_MOTOR_1,HIGH);
      digitalWrite(SIDE_MOTOR_2,LOW);
      digitalWrite(SIDE_MOTOR_3,LOW);
      digitalWrite(SIDE_MOTOR_4,HIGH);      
  }
  if(MOVMENT == MOV_RIGHT)
  {   digitalWrite(FORNT_MOTOR_1,LOW);
      digitalWrite(FORNT_MOTOR_2,HIGH);
      digitalWrite(FORNT_MOTOR_3,HIGH);
      digitalWrite(FORNT_MOTOR_4,LOW);
      digitalWrite(SIDE_MOTOR_1,LOW);
      digitalWrite(SIDE_MOTOR_2,HIGH);
      digitalWrite(SIDE_MOTOR_3,HIGH);
      digitalWrite(SIDE_MOTOR_4,LOW);
  }
  if(MOVMENT == MOV_STOP)
  {   digitalWrite(FORNT_MOTOR_1,LOW);
      digitalWrite(FORNT_MOTOR_2,LOW);
      digitalWrite(FORNT_MOTOR_3,LOW);
      digitalWrite(FORNT_MOTOR_4,LOW);
      digitalWrite(SIDE_MOTOR_1,LOW);
      digitalWrite(SIDE_MOTOR_2,LOW);
      digitalWrite(SIDE_MOTOR_3,LOW);
      digitalWrite(SIDE_MOTOR_4,LOW);
  }
  if(MOVMENT == MOV_ANTICLK)
  {   digitalWrite(FORNT_MOTOR_1,HIGH);
      digitalWrite(FORNT_MOTOR_2,LOW);
      digitalWrite(FORNT_MOTOR_3,LOW);
      digitalWrite(FORNT_MOTOR_4,HIGH);
      digitalWrite(SIDE_MOTOR_1,LOW);
      digitalWrite(SIDE_MOTOR_2,HIGH);
      digitalWrite(SIDE_MOTOR_3,HIGH);
      digitalWrite(SIDE_MOTOR_4,LOW);
  }
  if(MOVMENT == MOV_CLK)
  {   digitalWrite(FORNT_MOTOR_1,LOW);
      digitalWrite(FORNT_MOTOR_2,HIGH);
      digitalWrite(FORNT_MOTOR_3,HIGH);
      digitalWrite(FORNT_MOTOR_4,LOW);
      digitalWrite(SIDE_MOTOR_1,HIGH);
      digitalWrite(SIDE_MOTOR_2,LOW);
      digitalWrite(SIDE_MOTOR_3,LOW);
      digitalWrite(SIDE_MOTOR_4,HIGH);
  }
}

