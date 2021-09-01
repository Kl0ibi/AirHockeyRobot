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



//ISR(TIMER1_COMPA_vect)              //use for other projects
//{
// //PORT_DIR_L|=(1<<PIN_DIR_L);
// //PORT_DIR_R|=(1<<PIN_DIR_R);
//}

ISR(TIMER3_COMPA_vect)      //toggle Pul pins
{
  PORT_PUL_L^=(1<<PIN_PUL_L);
  PORT_PUL_R^=(1<<PIN_PUL_R);
  cntsteps++;

}


