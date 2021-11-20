#include "constants.h"

//attach all Interrupts
void attachInterrupts() {
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_L_NUM), checkSwitchmidL,FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_R_NUM), checkSwitchmidR,FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
}


//detach all Interrupts
void detachInterrupts() {
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_R));
  detachInterrupt(digitalPinToInterrupt(SWITCH_TB));
}

//Go into error state and stop all steppers
void errorTrigger()  {
  error=true;
  Serial.println("Error");
  
  Stepperstop();
  detachInterrupts();
}

void checkDriverError()
{
   if ((!digitalRead(DRIVER_FLT_L)) || (!digitalRead(DRIVER_FLT_R))) { //check if there is a driver fault
    if (!error_drivers) {
      int i = 0;
      while (!(digitalRead(DRIVER_FLT_L)) || !(digitalRead(DRIVER_FLT_R))) {  //check if there is a real failure or induction fail
        if (++i >= INDUCTION_DRIVER)  {
            error_drivers = true;
            Serial.println("Error Drivers");
            errorTrigger();
            return;
        }
      }
    }
  }else {
    if (digitalRead(DRIVER_FLT_L) && digitalRead(DRIVER_FLT_R)) 
      error_drivers = false;
  }
}


//check Switches for falling edge and waur dir debounce
void checkSwitchmidL(){
  int i=0;
  while(!bitRead(PIN_SWITCH_MID_L,SWITCH_MID_L) && !state_switch_mid_l)
  {
    if(++i >= DEBOUNCE_SWITCH) 
    {
      Serial.println("Switch middle left hit");
      state_switch_mid_l=true;
      return;
    }
  }
}

void checkSwitchmidR(){
  int i=0;
  while(!bitRead(PIN_SWITCH_MID_R,SWITCH_MID_R) && !state_switch_mid_r)
  {
    if(++i >= DEBOUNCE_SWITCH) 
    {
      Serial.println("Switch middle right hit");
      state_switch_mid_r=true;
      return;
    }
  }
}

void checkSwitchTB(){
  int i=0;
  while(!bitRead(PIN_SWITCH_TB,SWITCH_TB) && !state_switch_tb)
  {
    if(++i >= DEBOUNCE_SWITCH) 
    {
      Serial.println("Switch top or bottom hit");
      state_switch_tb=true;
      return;
    }
  }
}


//Use to send an Array to Raspberry 
/*
  static int data[3|={x,y,isHomed};
  -0 position X
  -1 postion Y
  -2 homed 0/1
 
*/
void sendDatatoRaspberry()
{
  static int data[3]= {0,0,0};
  //static int temp_data[2]= {0,0};

  if(error==false)
  {
    

if((int)x != data[0] || (int)y != data[1])
{
  data[0]=x;
  data[1]=y;

  

  if(homed==true)
  {
    data[2]=1;
  }else{
    data[2]=0;
  }


    
  Serial.println(String(data[0]) + "," + String(data[1]) + "," + String(data[2]));
}
 
  }else {

    if(error_printed==false)
    {
      Serial.println("Error unplug Table wait 5sec and try again!");
      error_printed=true;
      
    }
  }
}


ISR(TIMER3_COMPA_vect)      //toggle Pul pins
{
  PORT_PUL_L^=(1<<PIN_PUL_L);
  PORT_PUL_R^=(1<<PIN_PUL_R);
  cntsteps++;

}

//64MHz timer use to check for goal or errrors
ISR(TIMER4_COMPB_vect)
{
  sendDatatoRaspberry();
}


void goal()
{
  Serial.println("Goal");
  //ledblink();
  //playGoalSound();
  SteppermovetoXY(100,100);
  //pushSolenoid();
  delay(700);
  SteppermovetoXY(DEFAULT_X,DEFAULT_Y);
}

void fans()
{
  if(fansOn==true)
  {
    digitalWrite(PIN_FANS,LOW);
    fansOn=false;
  }else
  {
    digitalWrite(PIN_FANS,HIGH);
    fansOn=true;
  }
}

void demo()
{
  SteppermovetoXY(100,100);
  SteppermovetoXY(400,270);
  SteppermovetoXY(400,100);
  SteppermovetoXY(100,100);
  SteppermovetoXY(400,270);
  SteppermovetoXY(400,100);
  SteppermovetoXY(DEFAULT_X,DEFAULT_Y);
  fans();
  delay(2000);
  fans();
  goal();
  doHoming();
  Serial.println("Demo finished");
  
}

void cycleTest()
{
  
  Serial.println("Cycle-Test start");
  setDefaultParams();
   
  //attach Interrupt to Switch
  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
 
  while(!state_switch_tb)
  {
    Steppermovebackward();    //move backwards until the Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_TB));   //detach Interrupt

  delay(500);
  state_switch_tb=false;
  
  
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_L_NUM), checkSwitchmidL,FALLING);
  

  while(!state_switch_mid_l)
  {
    Steppermoveleft();        //move to left until the left Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));

  delay(500);
  state_switch_mid_l=false;

   //attach Interrupt to Switch
  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
 
  while(!state_switch_tb)
  {
    Steppermoveforward();    //move forward until the Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_TB));   //detach Interrupt

  delay(500);
  state_switch_tb=false;
  
    attachInterrupt(digitalPinToInterrupt(SWITCH_MID_R_NUM), checkSwitchmidR,FALLING);
  
  while(!state_switch_mid_r)
  {
    Steppermoveleft();        //move to right until the left Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));

  delay(500);
  state_switch_mid_r=false;

  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
 
  while(!state_switch_tb)
  {
    Steppermovebackward();    //move backwards until the Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_TB));   //detach Interrupt
  
  delay(500);
  state_switch_tb=false;
  
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_L_NUM), checkSwitchmidL,FALLING);
 
  while(!state_switch_mid_l)
  {
    Steppermoveleft();        //move to left until the left Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
 
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));


  state_switch_tb=false;
  state_switch_mid_l=false;
  state_switch_mid_r=false;
  
  y=BARRIER_Y_MIN;  
  x=BARRIER_X_MIN;
 
  SteppermovetoXY(DEFAULT_X,DEFAULT_Y); //after homing move to Default position
}


void pushSolenoid()
{
  
}

void ledBlink()
{
  
}

void leds()
{
  
}


