  #include <TimerOne.h>
  #define Max_Pulse_Periodic_Time       2              //--> 4800 RPM --> 2.5 sec/m 
  
  #define Adjusting_mode_Low_Speed      10             //mSec
  #define Adjusting_mode_Med_Speed      1               //mSec  
  #define Adjusting_mode_Fast_Speed     100             //uSec

  #define Adjusting_mode_Low_turns      0.25
  #define Adjusting_mode_Med_turns      2    
  #define Adjusting_mode_Fast_turns     5
  
  
  volatile unsigned long IntCount = 0, 
  PulseCount = 0, SigRef=0,SigRefcm=0, 
  Pulse_Periodic_Time=2, Start_Pulses=0,
  Dir=0; 
 
  char buffer[] = {' ',' ',' '};                      // Receive 3 bytes
  
  const int servo = 13; 
  const int servo_dir = 7;

  
  void setup(void)
  {
    pinMode(servo, OUTPUT);
    pinMode(servo_dir, OUTPUT);
    Timer1.initialize(60);
    Timer1.attachInterrupt(ServoInt); 
    Serial.begin(4800);
  }

    
  void ServoInt(void)
  { 
     if (PulseCount<(20001)&&PulseCount>=0&&SigRef<110)
     {

          
          if (SigRefcm>PulseCount)                    //Moving Right
          {        
              Move_Right();
              Dir=1;
          }
      
          if (SigRefcm<PulseCount)                    //Moving Left
          {
              Move_left();
              Dir=2;
          }   
      }    
      
      else                                           //Out of Range
       {
        digitalWrite(servo, LOW);
        Dir=0;
       } 
  }

 
  void loop(void)
  {
    while (Serial.available() > 0) 
    {
        Serial.readBytes(buffer, 3);
        SigRef=(atoi(buffer));
    }  
        switch(SigRef)
        {
          case 510:                                      //Slow Right
            noInterrupts();
            digitalWrite(servo_dir, HIGH);
            
            for (int i=0;i<(Adjusting_mode_Low_turns*200);i++)
            {
                 digitalWrite(servo, HIGH);       
                 delay(Adjusting_mode_Low_Speed);
                 digitalWrite(servo, LOW);    
                 delay(Adjusting_mode_Low_Speed);
            }
           SigRef=501;
          break;
    
    
          case 520:                                      //Slow Left
            noInterrupts();
            digitalWrite(servo_dir, LOW);
            
            for (int i=0;i<(Adjusting_mode_Low_turns*200);i++)
            {
                 digitalWrite(servo, HIGH);       
                 delay(Adjusting_mode_Low_Speed);
                 digitalWrite(servo, LOW);    
                 delay(Adjusting_mode_Low_Speed);
            }
           SigRef=501;
          break;
    
    
          case 530:                                      //Med Right
            noInterrupts();
            digitalWrite(servo_dir, HIGH);
            
            for (int i=0;i<(Adjusting_mode_Med_turns*200);i++)
            {
                 digitalWrite(servo, HIGH);       
                 delay(Adjusting_mode_Med_Speed);
                 digitalWrite(servo, LOW);    
                 delay(Adjusting_mode_Med_Speed);
            }
           SigRef=501;
          break;
    
    
          case 540:                                      //Med Left
            noInterrupts();
          digitalWrite(servo_dir, LOW);
          
          for (int i=0;i<(Adjusting_mode_Med_turns*200);i++)
          {
               digitalWrite(servo, HIGH);       
               delay(Adjusting_mode_Med_Speed);
               digitalWrite(servo, LOW);    
               delay(Adjusting_mode_Med_Speed);
          }
         SigRef=501;
        break;
  
  
        case 550:                                      //Fast Right
          noInterrupts();
          digitalWrite(servo_dir, HIGH);
          
          for (int i=0;i<(Adjusting_mode_Fast_turns*200);i++)
          {
               digitalWrite(servo, HIGH);       
               delayMicroseconds(Adjusting_mode_Fast_Speed);
               digitalWrite(servo, LOW);    
               delayMicroseconds(Adjusting_mode_Fast_Speed);
          }
         SigRef=501;
        break;
  
  
        case 560:                                      //Fast Left
          noInterrupts();
          digitalWrite(servo_dir, LOW);
          
          for (int i=0;i<(Adjusting_mode_Fast_turns*200);i++)
          {
               digitalWrite(servo, HIGH);       
               delayMicroseconds(Adjusting_mode_Fast_Speed);
               digitalWrite(servo, LOW);    
               delayMicroseconds(Adjusting_mode_Fast_Speed);
          }
         SigRef=501;
        break;
        
        default:
         interrupts(); 
         SigRefcm=SigRef*200; 
        break;  
      }  

      print_fun();
  }
    
    
    void print_fun()
    {
      Serial.print("Serial: ");
      Serial.println(buffer);
      Serial.flush();
      
      Serial.print("Serial_Flush: ");
      Serial.println(buffer);
       
      Serial.print("SigRefcm: ");
      Serial.println(SigRefcm);
      
      Serial.print("PulseCount: ");
      Serial.println(PulseCount);
      
      Serial.print("Pulse_Periodic_Time: ");
      Serial.println(Pulse_Periodic_Time);
      Serial.println(); 
    }
    

    void Move_Right()
  {
      digitalWrite(servo_dir, HIGH); 
      IntCount ++;
    
      //Accelerating_Pulses();     
      //Decelerating_Pulses_R();                               
      
      if(IntCount==Pulse_Periodic_Time)
      {  
        digitalWrite(servo, HIGH);       
      }
       
      if(IntCount==(2*Pulse_Periodic_Time))
      {  
        digitalWrite(servo, LOW);    
        IntCount=0;
        PulseCount++;
        Start_Pulses++;             
      }  
  }


    void Move_left()
  {
      digitalWrite(servo_dir, LOW); 
      IntCount ++;
  
      //Accelerating_Pulses();
      //Decelerating_Pulses_L();           
        
      if(IntCount==Pulse_Periodic_Time)
      { 
          digitalWrite(servo, HIGH);       
      }
      if(IntCount==(2*Pulse_Periodic_Time))
      {  
         digitalWrite(servo, LOW);    
         IntCount=0;
         PulseCount--;
         Start_Pulses++; 
         
         if(Pulse_Periodic_Time >Max_Pulse_Periodic_Time)
         {
             Pulse_Periodic_Time--;
         }
      }      
  }

  
  void Accelerating_Pulses()
  {
    if (Start_Pulses <100)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 4;
    
    else if (Start_Pulses <200)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 3;   
  
    else if (Start_Pulses <300)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 2;
        
    else if (Start_Pulses <400)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 2;    
    
    else if (Start_Pulses <500)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 1;
    
    else if (Start_Pulses <600)          
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time+ 1; 
    
    else if (Start_Pulses >600)         
        Pulse_Periodic_Time = Max_Pulse_Periodic_Time;  
  }


void Decelerating_Pulses_R()
  {
      if (SigRefcm - PulseCount <200)          
      {   
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 7;
          Start_Pulses=0;
      }
      else if (SigRefcm - PulseCount <400)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 6;   
  
      else if (SigRefcm - PulseCount <600)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 5;
          
      else if (SigRefcm - PulseCount <800)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 4;    
      
      else if (SigRefcm - PulseCount <1000)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 3;
      
      else if (SigRefcm - PulseCount <1200)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 2;   
  
      else if (SigRefcm - PulseCount <1400)          
          Pulse_Periodic_Time=Max_Pulse_Periodic_Time+ 1;
  }


void Decelerating_Pulses_L()
  {
    if (PulseCount - SigRefcm <100)          
    {
        Pulse_Periodic_Time=10;
        Start_Pulses=0;
    }
  
    else if (PulseCount - SigRefcm <200)          
        Pulse_Periodic_Time=5;
    
    else if (PulseCount - SigRefcm <300)          
        Pulse_Periodic_Time=4;
  
    else if (PulseCount - SigRefcm <400)          
        Pulse_Periodic_Time=3;    
    
    else if (PulseCount - SigRefcm <500)          
        Pulse_Periodic_Time=3;
  
    else if (PulseCount - SigRefcm <600)          
        Pulse_Periodic_Time=2;
    
    else if (PulseCount - SigRefcm <700)          
        Pulse_Periodic_Time=2;          
  }
