
/*This code is open source and can be modified by the permission of author and company is need be
 
  Written by - Vyom Rajan Singh
  Company - IDR R&D Pvt Ltd

  USE: controls pan and tilt servos and uses IBUS to use another 2 channels for special function.

  GOOD DAY!!

  Contributors:
  *
  *
  
                                                                                                                                  */

#include <IBusBM.h>
#include <Servo.h>


IBusBM IBus; 

Servo Panservo;  
Servo Tiltservo;

int savevalpan = 90;
int savevaltilt = 90;
int flag = 0, flag1 = 0;
int iti_delay = 500;
int savevalreset = 0;
int pan, pan1, tilt, tilt1, reset, TdB, Timer1 = 0;

void setup() {
  // Serial.begin(115200);  

  IBus.begin(Serial);    

  Panservo.attach(9);
  Tiltservo.attach(10);

  Serial.println("Start IBus2PWM");

                                                  //initialization sequence
  Panservo.write(90);
  Tiltservo.write(90);
  
  Serial.print("90");                                    //condition to write pan/tilt if val changes
  Serial.print("  ");
  Serial.println("90"); 
  
  delay(iti_delay);
  
  Serial.print("90");                                    //condition to write pan/tilt if val changes
  Serial.print("  ");
  Serial.println("90"); 
  
}

void loop() 
{
  
  
  pan = mapservo(IBus.readChannel(4)); 
  tilt = mapservo(IBus.readChannel(5));                    //Ibus data aquisition
  reset = IBus.readChannel(9);
  TdB = IBus.readChannel(8);
  
  delay(10);

  pan1 = mapservo(IBus.readChannel(4)); 
  tilt1 = mapservo(IBus.readChannel(5));                    //eliminating jitters
  
  pan = jitters_avg(pan, pan1);
  tilt = jitters_avg(tilt, tilt1);
  

   if (reset < 1900)
   {
    if (savevalpan != pan || savevaltilt != tilt)  
    {

     Panservo.write(pan);  
     Tiltservo.write(tilt);
     
     Serial.print(pan);                                    //condition to write pan/tilt if val changes
     Serial.print("  ");
     Serial.println(tilt); 
     
     savevalpan = pan;
     savevaltilt = tilt;
        

     //flag = 0;
    }
   }
   if(reset > 1900)                                         // if reset switch is triggered
   {  
      if(flag == 0)
      {
        Panservo.write(90);  
        Tiltservo.write(90);
        
        Serial.print("90");                                   
        Serial.print("  ");
        Serial.println("90"); 
            

        flag = 1;
      }
   }

   if (savevalpan != pan || savevaltilt != tilt)
   {
    if (reset > 1900)
    { 

      Panservo.write(pan);  
      Tiltservo.write(tilt);
      
      Serial.print(pan);                                    //condition to write pan/tilt if val changes
      Serial.print("  ");
      Serial.println(tilt); 
       
      savevalpan = pan;
      savevaltilt = tilt;
         


    }
   }
      if(reset>1900)                                          // timebound reset if no change is detected and reset is ON
      {
        if(savevalpan == pan || savevaltilt == tilt)
        {
          Timer1++;
        }

      }
      if (Timer1 == 250)
      {
        Timer1 = 0;
        flag = 0;
      }

   if (TdB > 1900 && flag1 == 0)                              // Look down switch
   {
    Panservo.write(90);
    Tiltservo.write(170);

    Serial.print("90");                                    //condition to write pan/tilt if val changes
    Serial.print("  ");
    Serial.println("170");
    flag1 = 1;
   }
   if(TdB<1900)
    {
      flag1 = 0;
    }
   
   
   
}

/* User defined functions*/

int mapservo(int val)                       //to map the servos                                               
{
  val = map(val, 1000, 2000, 10, 170);
  return val;
}

int jitters_avg(int x, int y)               // elimination pot inacurracy with averaging      
{
  int temp;
  temp = (x+y)/2;
  return temp;
}
