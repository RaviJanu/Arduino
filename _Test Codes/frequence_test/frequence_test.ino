//!!AUM!!
//This File Contain SD card funtions n code
/*  
*SD card attached to SPI bus as follows:
**MOSI  -   pin 11 uno -   pin  51 mega
**MISO  -   pin 12     -   pin  50
**CLK   -   pin 13     -   pin  52 
**CS    -   pin 10     -   pin  53
*/
#define Speaker_Out   7  

long Set_Freq = 50000, Freq_counter = 50000,normal_counter = 0;

void setup() 
{   Serial.begin(9600);
    pinMode(Speaker_Out,OUTPUT);
}

long base_us_delay = 0;
char flag = 0,data_size = 0;

void loop() 
{ 
  while(1)
  { digitalWrite(Speaker_Out,HIGH);
    delayMicroseconds(Set_Freq);
    digitalWrite(Speaker_Out,LOW);
    delayMicroseconds(Set_Freq);
    if(Serial.available() > 0)  break;
  }
    String Data = readData();
    char str_to_char_buff[10] = "";
    long temp = 0, time_value = 0;; 
    Data.toCharArray(str_to_char_buff,data_size+1);
    for(char i = 0; i < data_size ; i++)
    { temp *= 10;
      temp += (str_to_char_buff[i] - 0x30);
    }
    Set_Freq = 1000000 / temp;
    
    Serial.println(Set_Freq,DEC);
}

String readData(void)        //read incoming data into string, through uart
{   String data = "";
    unsigned long t = millis();
    char i = 0;
    while(millis() - t < 500) 
    { if(Serial.available() > 0) 
      {   char r = Serial.read();
          data += r;  
          t = millis();
          i++;
          data_size = i;
      }
    }
    Serial.println(data);
    return data;
}



/*
 *   if(normal_counter < Freq_counter)
  { normal_counter = 0;
    if(flag == 0) 
    {   flag = 1;
        digitalWrite(Speaker_Out,HIGH);
    }
    else
    {   flag = 0;
        digitalWrite(Speaker_Out,LOW);
    }
  }
  normal_counter++;
    if(Freq_counter < Set_Freq - 15) Freq_counter += 10;
  if(Freq_counter > Set_Freq + 15) Freq_counter -= 10;

  if(Serial.available() > 0)
  { String Data = readData();
    char str_to_char_buff[10] = "";
    long temp = 0, time_value = 0;; 
    Data.toCharArray(str_to_char_buff,data_size+1);
    for(char i = 0; i < data_size ; i++)
    { temp *= 10;
      temp += (str_to_char_buff[i] - 0x30);
    }
    Set_Freq = 1000000 / temp;
    Serial.println(Set_Freq,DEC);
  }
 */




