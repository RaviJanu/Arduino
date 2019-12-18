//!!AUM!!
//INIT...

#define POT_1     A0
#define POT_2     A1
#define POT_3     A2

#define TRESHOLD  10

int PastVal_1 = 0,PastVal_2 = 0,PastVal_3 = 0;

void setup() 
{ // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() 
{ int CurrentVal_1 = 0,CurrentVal_2 = 0,CurrentVal_3 = 0;
  char SendArray = 0;

  CurrentVal_1 = analogRead(POT_1);
  CurrentVal_2 = analogRead(POT_2);
  CurrentVal_3 = analogRead(POT_3);

  if(CurrentVal_1 <= 350)       Serial.println('A');    //SendArray = 'A';
  if(CurrentVal_1 >= 850)       Serial.println('B');   //SendArray = 'B';
  if(CurrentVal_2 <= 470)       Serial.println('C');   //SendArray = 'C';
  if(CurrentVal_2 >= 900)       Serial.println('D');   //SendArray = 'D';
  if(CurrentVal_3 <= 570)       Serial.println('E');   //SendArray = 'E';
  if(CurrentVal_3 >= 900)       Serial.println('F');   //SendArray = 'F';

  delay(100);
//  Serial.println(CurrentVal_3);
//  delay(20);
}
