//!!AUM!!
#define THUMB_FLEX    3
#define INDEX_FLEX    4
#define MIDDLE_FLEX   5
#define RING_FLEX     6
#define PINKY_FLEX    7

void setup() 
{ // put your setup code here, to run once:
  Serial.begin(9600); 
}

void loop()
{ // put your main code here, to run repeatedly:
  if(Serial.available())
  { if(Serial.read() == '@')
    { int ThumbFlex = 0,IndexFlex = 0,MiddleFlex = 0,RingFlex = 0,PinkyFlex = 0;
      ThumbFlex   = analogRead(THUMB_FLEX);
      IndexFlex   = analogRead(INDEX_FLEX);
      MiddleFlex  = analogRead(MIDDLE_FLEX);
      RingFlex    = analogRead(RING_FLEX);
      PinkyFlex   = analogRead(PINKY_FLEX);
      Serial.print(String(ThumbFlex)+String(IndexFlex)+String(MiddleFlex)+String(RingFlex)+String(PinkyFlex)+"_");
    }
  }
}
