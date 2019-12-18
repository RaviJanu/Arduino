//!!AUM!!
//INIT...
//It is Line Followor and delay based robot for Library Managemant.
//Common was recev from RF module for a perticular location and it will go there come back with text book as per delay istruction


#define LEFT_MOTOR_1        2
#define LEFT_MOTOR_2        3
#define RIGHT_MOTOR_1       4
#define RIGHT_MOTOR_2       5
#define UD_MOTOR_1          6
#define UD_MOTOR_2          7
#define OC_MOTOR_1          8
#define OC_MOTOR_2          9

#define RIGHT_IR            10
#define MID_IR              11
#define LEFT_IR             12

#define RFID_1              A0    
#define RFID_2              A1
#define RFID_3              A2
#define RFID_4              A3

#define MOV_STOP          'X'
#define MOV_FORWARD       'Q'
#define MOV_BACKWARD      'W'
#define MOV_LEFT          'E'
#define MOV_RIGHT         'R'
#define MOV_UP            'T'
#define MOV_DOWN          'Y'
#define MOV_CLOSE         'U'
#define MOV_OPEN          'I'
#define STATION_A         'A'
#define STATION_B         'S'
#define STATION_C         'D'
#define STATION_D         'F'

#define STATION_W         'G'
#define STATION_X         'H'
#define STATION_Y         'J'
#define STATION_Z         'K'

#define WHITE               1
#define BLACK               0
#define MAIN                0
#define BRANCH              1


#define MAINROOT_FORWARD_DELAY      5000
#define LR_90_DELAY                 3300
#define BRANCH_FORWARD_DELAY        2000
#define PIC_DROP_DELAY              2500
#define UP_DOWN_DELAY               3000
#define LR_180_DELAY                LR_90_DELAY*2

char StationName = 0,StationNameDelayBase = 0; 

void setup() 
{  // put your setup code here, to run once:
  pinMode(LEFT_MOTOR_1,OUTPUT);
  pinMode(LEFT_MOTOR_2,OUTPUT);
  pinMode(RIGHT_MOTOR_1,OUTPUT);
  pinMode(RIGHT_MOTOR_2,OUTPUT);
  pinMode(UD_MOTOR_1,OUTPUT);
  pinMode(UD_MOTOR_2,OUTPUT);
  pinMode(OC_MOTOR_1,OUTPUT);
  pinMode(OC_MOTOR_2,OUTPUT);
  pinMode(RIGHT_IR,INPUT);
  pinMode(LEFT_IR,INPUT);
  pinMode(MID_IR,INPUT);
  pinMode(RFID_1,INPUT);    
  pinMode(RFID_2,INPUT);    
  pinMode(RFID_3,INPUT);    
  pinMode(RFID_4,INPUT);       
  
  ROBOT_MOVMENT(MOV_STOP);
  Serial.begin(9600);
//  Serial.println("Robot Start");
}

void loop() 
{   SERIAL_CMD();
    gotoStation();
    gotoStationDelayBase();

    byte RightIR = 0,LeftIR = 0,MidIR = 0,CheckPoint = 0;

    RightIR = digitalRead(RIGHT_IR);
    LeftIR = digitalRead(LEFT_IR);
    MidIR = digitalRead(MID_IR);
  //  Serial.println("L"+String(LeftIR)+"M"+String(MidIR)+"R"+String(RightIR));
    delay(10);
}

void gotoStation(void)
{ if(StationName == 0)  return;

  LineFollow(MAIN);
  if((StationName == 1)||(StationName == 3))        ROBOT_MOVMENT(MOV_LEFT);
  if((StationName == 2)||(StationName == 4))        ROBOT_MOVMENT(MOV_RIGHT);
  delay(LR_90_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  ROBOT_MOVMENT(MOV_FORWARD);
  delay(1000);
  LineFollow(BRANCH);
  ROBOT_MOVMENT(MOV_STOP);
  while(1)
  { byte rfid_recv = RFID_RX();
    if((StationName == 1)&&(rfid_recv == 0x01))   break;
    if((StationName == 2)&&(rfid_recv == 0x02))   break;
    if((StationName == 3)&&(rfid_recv == 0x04))   break;
    if((StationName == 4)&&(rfid_recv == 0x08))   break;
    delay(1);
  }
  ROBOT_MOVMENT(MOV_CLOSE);
  delay(PIC_DROP_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  ROBOT_MOVMENT(MOV_UP);
  delay(UP_DOWN_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  if((StationName == 1)||(StationName == 3))        ROBOT_MOVMENT(MOV_LEFT);
  if((StationName == 2)||(StationName == 4))        ROBOT_MOVMENT(MOV_RIGHT);
  delay(LR_90_DELAY);
  ROBOT_MOVMENT(MOV_FORWARD);
  
  if((StationName == 1)||(StationName == 2))        delay(MAINROOT_FORWARD_DELAY);
  if((StationName == 3)||(StationName == 4))        delay(MAINROOT_FORWARD_DELAY*2);
  ROBOT_MOVMENT(MOV_STOP);

  ROBOT_MOVMENT(MOV_DOWN);
  delay(UP_DOWN_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  ROBOT_MOVMENT(MOV_OPEN);
  delay(PIC_DROP_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  if((StationName == 1)||(StationName == 3))        ROBOT_MOVMENT(MOV_RIGHT);
  if((StationName == 2)||(StationName == 4))        ROBOT_MOVMENT(MOV_LEFT);
  delay(LR_180_DELAY);
  ROBOT_MOVMENT(MOV_STOP); 
  
  StationName = 0;
}

void LineFollow(char main_branch)
{ byte RightIR = 0,LeftIR = 0,MidIR = 0,CheckPoint = 0;

//  Serial.println("Line follow start");
  while(1)
  { RightIR = digitalRead(RIGHT_IR);
    LeftIR = digitalRead(LEFT_IR);
    MidIR = digitalRead(MID_IR);
 //     Serial.println("L"+String(LeftIR)+"M"+String(MidIR)+"R"+String(RightIR));

    if((LeftIR == WHITE)&&(MidIR == WHITE)&&(RightIR == WHITE))        ROBOT_MOVMENT(MOV_FORWARD);
    if((LeftIR == WHITE)&&(MidIR == BLACK)&&(RightIR == WHITE))        ROBOT_MOVMENT(MOV_FORWARD);
    if((LeftIR == WHITE)&&(MidIR == BLACK)&&(RightIR == BLACK))        ROBOT_MOVMENT(MOV_RIGHT);
    if((LeftIR == WHITE)&&(MidIR == WHITE)&&(RightIR == BLACK))        ROBOT_MOVMENT(MOV_RIGHT);
    if((LeftIR == BLACK)&&(MidIR == BLACK)&&(RightIR == WHITE))        ROBOT_MOVMENT(MOV_LEFT);
    if((LeftIR == BLACK)&&(MidIR == WHITE)&&(RightIR == WHITE))        ROBOT_MOVMENT(MOV_LEFT);
    if((LeftIR == BLACK)&&(MidIR == BLACK)&&(RightIR == BLACK))        
    { CheckPoint++;
  //    Serial.println(CheckPoint);
      ROBOT_MOVMENT(MOV_STOP);
      ROBOT_MOVMENT(MOV_FORWARD);
      delay(800);
      ROBOT_MOVMENT(MOV_STOP);
      if((main_branch == BRANCH)&&(CheckPoint == 1))     return;                 
      if((StationName > 2)&&(CheckPoint == 2))          return;
      if((StationName <= 2)&&(CheckPoint == 1))         return;
    }
    delay(2);
    digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,LOW);
    delay(2); 
    ROBOT_MOVMENT(MOV_FORWARD);
    delay(2); 
  }
  
}


unsigned char RFID_RX(void)
{ unsigned char rx_data = 0;
  if(digitalRead(RFID_1) == 1) rx_data |= 0x01;
  if(digitalRead(RFID_2) == 1) rx_data |= 0x02;
  if(digitalRead(RFID_3) == 1) rx_data |= 0x04;
  if(digitalRead(RFID_4) == 1) rx_data |= 0x08;
  return(rx_data);
}



void SERIAL_CMD(void)
{   if(!Serial.available())    return; 
    char Serial_Data = Serial.read();
    Serial.print(Serial_Data);
    ROBOT_MOVMENT(Serial_Data);
    if(Serial_Data == STATION_A)      StationName = 1;
    if(Serial_Data == STATION_B)      StationName = 2;
    if(Serial_Data == STATION_C)      StationName = 3;
    if(Serial_Data == STATION_D)      StationName = 4;
    
    if(Serial_Data == STATION_W)      StationNameDelayBase = 1;
    if(Serial_Data == STATION_X)      StationNameDelayBase = 2;
    if(Serial_Data == STATION_Y)      StationNameDelayBase = 3;
    if(Serial_Data == STATION_Z)      StationNameDelayBase = 4;
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
  if(MOVMENT == MOV_UP)
  {   digitalWrite(UD_MOTOR_1,LOW);
      digitalWrite(UD_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_DOWN)
  {   digitalWrite(UD_MOTOR_2,LOW);
      digitalWrite(UD_MOTOR_1,HIGH);
  }
  if(MOVMENT == MOV_OPEN)
  {   digitalWrite(OC_MOTOR_1,LOW);
      digitalWrite(OC_MOTOR_2,HIGH);
  }
  if(MOVMENT == MOV_CLOSE)
  {   digitalWrite(OC_MOTOR_2,LOW);
      digitalWrite(OC_MOTOR_1,HIGH);
  }

  if(MOVMENT == MOV_STOP)
  {   digitalWrite(LEFT_MOTOR_1,LOW);
      digitalWrite(LEFT_MOTOR_2,LOW);
      digitalWrite(RIGHT_MOTOR_1,LOW);
      digitalWrite(RIGHT_MOTOR_2,LOW);
      digitalWrite(UD_MOTOR_1,LOW);
      digitalWrite(UD_MOTOR_2,LOW);
      digitalWrite(OC_MOTOR_1,LOW);
      digitalWrite(OC_MOTOR_2,LOW);
      delay(1000);
  }
}




void gotoStationDelayBase(void)
{ if(StationNameDelayBase == 0)  return;

  ROBOT_MOVMENT(MOV_FORWARD);
  if((StationNameDelayBase == 1)||(StationNameDelayBase == 2))        delay(MAINROOT_FORWARD_DELAY);
  if((StationNameDelayBase == 3)||(StationNameDelayBase == 4))        delay(MAINROOT_FORWARD_DELAY*2);
  ROBOT_MOVMENT(MOV_STOP);
  
  if((StationNameDelayBase == 1)||(StationNameDelayBase == 3))        ROBOT_MOVMENT(MOV_LEFT);
  if((StationNameDelayBase == 2)||(StationNameDelayBase == 4))        ROBOT_MOVMENT(MOV_RIGHT);
  delay(LR_90_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  ROBOT_MOVMENT(MOV_FORWARD);
  delay(BRANCH_FORWARD_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  ROBOT_MOVMENT(MOV_CLOSE);
  delay(PIC_DROP_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  ROBOT_MOVMENT(MOV_UP);
  delay(UP_DOWN_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  if((StationNameDelayBase == 1)||(StationNameDelayBase == 3))        ROBOT_MOVMENT(MOV_LEFT);
  if((StationNameDelayBase == 2)||(StationNameDelayBase == 4))        ROBOT_MOVMENT(MOV_RIGHT);
  delay(LR_180_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  ROBOT_MOVMENT(MOV_FORWARD);
  delay(BRANCH_FORWARD_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  
  if((StationNameDelayBase == 1)||(StationNameDelayBase == 3))        ROBOT_MOVMENT(MOV_RIGHT);
  if((StationNameDelayBase == 2)||(StationNameDelayBase == 4))        ROBOT_MOVMENT(MOV_LEFT);
  delay(LR_90_DELAY);
  ROBOT_MOVMENT(MOV_STOP); 

  ROBOT_MOVMENT(MOV_FORWARD);
  if((StationNameDelayBase == 1)||(StationNameDelayBase == 2))        delay(MAINROOT_FORWARD_DELAY);
  if((StationNameDelayBase == 3)||(StationNameDelayBase == 4))        delay(MAINROOT_FORWARD_DELAY*2);
  ROBOT_MOVMENT(MOV_STOP);

  ROBOT_MOVMENT(MOV_DOWN);
  delay(UP_DOWN_DELAY);
  ROBOT_MOVMENT(MOV_STOP);
  ROBOT_MOVMENT(MOV_OPEN);
  delay(PIC_DROP_DELAY);
  ROBOT_MOVMENT(MOV_STOP);

  if((StationNameDelayBase == 1)||(StationNameDelayBase == 3))        ROBOT_MOVMENT(MOV_RIGHT);
  if((StationNameDelayBase == 2)||(StationNameDelayBase == 4))        ROBOT_MOVMENT(MOV_LEFT);
  delay(LR_180_DELAY);
  ROBOT_MOVMENT(MOV_STOP); 
  StationNameDelayBase = 0;
}
