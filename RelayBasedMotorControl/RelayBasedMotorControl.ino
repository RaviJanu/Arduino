//!!AUM!!

#define LEFT_RELAY_1        2
#define LEFT_RELAY_2        3
#define RIGHT_RELAY_1       4
#define RIGHT_RELAY_2       5
#define X_ACC               A0
#define Y_ACC               A1

void setup() 
{ Serial.begin(9600);
  pinMode(LEFT_RELAY_1,OUTPUT); 
  pinMode(LEFT_RELAY_2,OUTPUT);
  pinMode(RIGHT_RELAY_1,OUTPUT);
  pinMode(RIGHT_RELAY_2,OUTPUT);
  digitalWrite(LEFT_RELAY_1,LOW);
  digitalWrite(LEFT_RELAY_2,LOW);
  digitalWrite(RIGHT_RELAY_1,LOW);
  digitalWrite(RIGHT_RELAY_2,LOW);
  delay(3000);
}

#define X_LOW       290
#define X_HIGH      390
#define Y_LOW       290
#define Y_HIGH      390

void loop() 
{ unsigned int X_acc_adc = 0,Y_acc_adc = 0;
  X_acc_adc = analogRead(X_ACC);
  Y_acc_adc = analogRead(Y_ACC);
  
  if(X_acc_adc < X_LOW)
  {   digitalWrite(LEFT_RELAY_1,LOW);
      digitalWrite(LEFT_RELAY_2,HIGH);
      digitalWrite(RIGHT_RELAY_1,LOW);
      digitalWrite(RIGHT_RELAY_2,HIGH);
      Serial.println("Backward");
  }
  if(X_acc_adc > X_HIGH)
  {   digitalWrite(LEFT_RELAY_1,HIGH);
      digitalWrite(LEFT_RELAY_2,LOW);
      digitalWrite(RIGHT_RELAY_1,HIGH);
      digitalWrite(RIGHT_RELAY_2,LOW);
      Serial.println("Forward");
  }
  if(Y_acc_adc < Y_LOW)
  {   digitalWrite(LEFT_RELAY_1,LOW);
      digitalWrite(LEFT_RELAY_2,HIGH);
      digitalWrite(RIGHT_RELAY_1,HIGH);
      digitalWrite(RIGHT_RELAY_2,LOW);
      Serial.println("RIGHT");
  }
  if(Y_acc_adc > Y_HIGH)
  {   digitalWrite(LEFT_RELAY_1,HIGH);
      digitalWrite(LEFT_RELAY_2,LOW);
      digitalWrite(RIGHT_RELAY_1,LOW);
      digitalWrite(RIGHT_RELAY_2,HIGH);
      Serial.println("LEFT");
  }
  if((X_acc_adc > X_LOW)&&(X_acc_adc < X_HIGH)&&(Y_acc_adc > Y_LOW)&&(Y_acc_adc < Y_HIGH))
  {   digitalWrite(LEFT_RELAY_1,LOW);
      digitalWrite(LEFT_RELAY_2,LOW);
      digitalWrite(RIGHT_RELAY_1,LOW);
      digitalWrite(RIGHT_RELAY_2,LOW);
      Serial.println("STOP");
  }
  
//  Serial.println("X "+String(X_acc_adc)+" Y "+String(Y_acc_adc));
  delay(100);
}
