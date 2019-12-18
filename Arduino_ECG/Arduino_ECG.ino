//!AUM!!

#define HEART_SENSOR        A0        //HEART Beat sensor analog pin name
#define MAX_FIFO_SIZE_H    10         //buffer size

int HeartReat_fifo[MAX_FIFO_SIZE_H];    //fifo HB sesnor analog value buffer
long HB_Avg_Value = 0;                  //average of of buffer value
int pulse_delay_count = 0;              //counter for pulse delay count
char beat_detect_flag = 0;              //one pulse detect for on time flag selection

void setup() 
{ Serial.begin(9600);                 //uart init
}

void loop() 
{ HeartBeat_Sensor();               //get heart beat analog value and calculate heart rate
  delay(50);                        //sample delay
}


void HeartBeat_Sensor(void)
{ int pulse_analog_value = 0,pulse_rate = 0,i = 0;
  pulse_analog_value = analogRead(HEART_SENSOR);      //get heartbeat analog value
  
  Serial.print("H"+String(pulse_analog_value)+"_");   //print analog value on uart
//  Serial.println(pulse_analog_value);

  if(pulse_analog_value > 700)                        // pulse peak threshold
  { if(beat_detect_flag == 0)                         //one pulse at on time
    { beat_detect_flag = 1;
      pulse_rate = 60000 / (pulse_delay_count*50);    //pulse rate  =  60000 / pulse_delay_count * 20;  1> pulse rate = pulse per minute 
                                                      //2>  1min = 60sec = 60000 mil sec
                                                      //3> our sampling rate = 20 mil sec
                                                      //4> pulse_delay_count = number of count btw to pulse detection
                                                      //5> so assume 40 count take between 2 pulse which (40*20 = 800)ms delay time between 2 pulse
                                                      //6> 600000 / time delay between 2 pulse = rate of pulse 
      if((pulse_rate > 50)&&(pulse_rate < 150))       //pulse rate count check
      { 
        for(i = 0;i<MAX_FIFO_SIZE_H - 1;i++)          //value shift inside buffer 
        { HeartReat_fifo[i] = HeartReat_fifo[i+1];    //value shift inside buffer
          HB_Avg_Value += HeartReat_fifo[i];          //value shift inside buffer
        }
        HeartReat_fifo[MAX_FIFO_SIZE_H-1] = pulse_rate;
        HB_Avg_Value += HeartReat_fifo[MAX_FIFO_SIZE_H-1];  //avg of past 10 vlaue
        HB_Avg_Value /= MAX_FIFO_SIZE_H;                //avg of past 10 vlaue
      }
      pulse_delay_count = 0;   
    }
  }
  else
  { beat_detect_flag = 0;
  }  
  
  pulse_delay_count++; 
  if(pulse_delay_count > 60)                                //if more time take to get value counter reset
  { pulse_delay_count = 0;
    for(i = 0;i<MAX_FIFO_SIZE_H;i++)                      //fifo clear
    { HeartReat_fifo[i] = 0;
      HB_Avg_Value = 0;
    }
  }
  Serial.println("R"+String(HB_Avg_Value)+"_");         //print heart rate on uart
  
}

