 //!!AUM!!
//INIT...

#define MOTOR_1PIN_1        2
#define MOTOR_1PIN_2        3
#define MOTOR_2PIN_1        4
#define MOTOR_2PIN_2        5
#define MOTOR_3PIN_1        6
#define MOTOR_3PIN_2        7
char serial_counter = 0,Recv_data = 0;

void setup() 
{  // put your setup code here, to run once:
  pinMode(MOTOR_1PIN_1,OUTPUT);
  pinMode(MOTOR_1PIN_2,OUTPUT);
  pinMode(MOTOR_2PIN_1,OUTPUT);
  pinMode(MOTOR_2PIN_2,OUTPUT);
  pinMode(MOTOR_3PIN_1,OUTPUT);
  pinMode(MOTOR_3PIN_2,OUTPUT);

  digitalWrite(MOTOR_1PIN_1,LOW);
  digitalWrite(MOTOR_1PIN_2,LOW);
  digitalWrite(MOTOR_2PIN_1,LOW);
  digitalWrite(MOTOR_2PIN_2,LOW);
  digitalWrite(MOTOR_3PIN_1,LOW);
  digitalWrite(MOTOR_3PIN_2,LOW);

  Serial.begin(9600);
  delay(5000);
}

#define COMMON_DELAY 100

void loop() 
{  // put your main code here, to run repeatedly:
  
  if(Serial.available() > 0)
  { char r = Serial.read();
    if(r == 'A')
    { digitalWrite(MOTOR_1PIN_1,HIGH);
      digitalWrite(MOTOR_1PIN_2,LOW);
      delay(COMMON_DELAY);
    }
    if(r == 'B')
    { digitalWrite(MOTOR_1PIN_2,HIGH);
      digitalWrite(MOTOR_1PIN_1,LOW);
      delay(COMMON_DELAY);
    }
    if(r == 'C')
    { digitalWrite(MOTOR_2PIN_1,HIGH);
      digitalWrite(MOTOR_2PIN_2,LOW);
      delay(COMMON_DELAY);
    }
    if(r == 'D')
    { digitalWrite(MOTOR_2PIN_2,HIGH);
      digitalWrite(MOTOR_2PIN_1,LOW);
      delay(COMMON_DELAY);
    }
    if(r == 'E')
    { digitalWrite(MOTOR_3PIN_1,HIGH);
      digitalWrite(MOTOR_3PIN_2,LOW);
      delay(COMMON_DELAY);
    }
    if(r == 'F')
    { digitalWrite(MOTOR_3PIN_2,HIGH);
      digitalWrite(MOTOR_3PIN_1,LOW);
      delay(COMMON_DELAY);
    }    
    digitalWrite(MOTOR_1PIN_1,LOW);
    digitalWrite(MOTOR_1PIN_2,LOW);
    digitalWrite(MOTOR_2PIN_1,LOW);
    digitalWrite(MOTOR_2PIN_2,LOW);
    digitalWrite(MOTOR_3PIN_1,LOW);
    digitalWrite(MOTOR_3PIN_2,LOW);
  }
}
