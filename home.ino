#include "constants.h"
void doHoming()
{

  Serial.println("Homing start");
  Serial.println("Homing Y-axis");
  Serial.println("");

  //attach Interrupt to Switch
  //attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB,FALLING);
  detachSwitchTB = false;
  detachSwitchmidL = false;
  while (!state_switch_tb)
  {
    Steppermovebackward();    //move backwards until the Button gets hit
    checkDriverError();
  }
  Stepperstop();

  detachSwitchTB = true;
  state_switch_tb = false;
  //detachInterrupt(digitalPinToInterrupt(SWITCH_TB));   //detach Interrupt


  y = BARRIER_Y_MIN;    //current Y coordinate is the minimal Y-barrier



  Serial.println("Homing X axis");

  while (!state_switch_mid_l)
  {
    Steppermoveleft();        //move to left until the left Button gets hit
    checkDriverError();
  }
  Stepperstop();

  detachSwitchmidL = true;
  state_switch_mid_l = false;

  //detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));

  x = BARRIER_X_MIN;  //current X coordinate is the minimal X-barrier

  Serial.println("Move to Default Position");

  SteppermovetoXY(DEFAULT_X, DEFAULT_Y); //after homing move to Default position

  homed = true;   //finished homing
  goal_state=false;

  if (fansOn == false)
  { fans();
  }

}



