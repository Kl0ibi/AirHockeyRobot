#include "constants.h"
void doHoming()
{
  Serial.println("Homing start");
  Serial.println("Homing Y-axis");
  //attach all Interrupt to Switch
  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
  
  while(!state_switch_tb)
  {
    Steppermovebackward();    //move backwards until the Button gets hit
    checkDriverError(); 
  }
  
  detachInterrupt(digitalPinToInterrupt(SWITCH_TB));   //detach Interrupt
  
  Stepperstop();

  y=BARRIER_Y_MIN;      //current Y coordinate is the minimal Y-barrier
  
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_L_NUM), checkSwitchmidL,FALLING);
  Serial.println("Homing X axis");
  
  while(!state_switch_mid_l)
  {
    Steppermoveleft();        //move to left until the left Button gets hit
    checkDriverError(); 
  }
  Stepperstop();
  
  detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));
  
  x=BARRIER_X_MIN;    //current X coordinate is the minimal X-barrier

  Serial.println("Move to Default Position");
 
  SteppermovetoXY(DEFAULT_X,DEFAULT_Y); //after homing move to Default position
  
  homed=true;     //finished homing
  
}



