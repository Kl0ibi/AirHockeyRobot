#include "constants.h"

#define resetswitch()    state_switch_tb = false;state_switch_mid_l = false;state_switch_mid_r = false

//attach all Interrupts
void attachInterrupts() {
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_L_NUM), checkSwitchmidL, FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_MID_R_NUM), checkSwitchmidR, FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_TB_NUM), checkSwitchTB, FALLING);
}


//detach all Interrupts
void detachInterrupts() {   //dont use arduino detachInterrupt they are too slow
  //detachInterrupt(digitalPinToInterrupt(SWITCH_MID_L));
  //detachInterrupt(digitalPinToInterrupt(SWITCH_MID_R));
  //detachInterrupt(digitalPinToInterrupt(SWITCH_TB));
  detachSwitchTB = true;
  detachSwitchmidL = true;
  detachSwitchmidR = true;
}

//Go into error state and stop all steppers
void errorTrigger()  {
  error = true;
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
  } else {
    if (digitalRead(DRIVER_FLT_L) && digitalRead(DRIVER_FLT_R))
      error_drivers = false;
  }
}


//check Switches for falling edge and debounce switches
void checkSwitchmidL() {
  int i = 0;
  if (detachSwitchmidL == false)
  {
    if (!bitRead(PIN_SWITCH_MID_L, SWITCH_MID_L && !state_switch_mid_l) ) //
    {
      Serial.println("Switch middle left hit");
      state_switch_mid_l = true;
      delay(150);
      EIFR = (1 << SWITCH_MID_L);
    }
  }
}

void checkSwitchmidR() {
  int i = 0;
  if (detachSwitchmidR == false)
  {
    if (!bitRead(PIN_SWITCH_MID_R, SWITCH_MID_R && !state_switch_mid_r) ) //
    {
      Serial.println("Switch middle right hit");
      state_switch_mid_r = true;
      delay(150);
      EIFR = (1 << SWITCH_MID_R);
    }
  }
}

void checkSwitchTB() {
  int i = 0;
  if (detachSwitchTB == false)
  {
    if (!bitRead(PIN_SWITCH_TB, SWITCH_TB && !state_switch_tb)) //
    {
      Serial.println("Switch top or bottom hit");
      state_switch_tb = true;
      delay(150);
      EIFR = (1 << SWITCH_TB);

    }
  }
}

void buttonPressed()
{
  if (!digitalRead(SWITCH_MID_L_NUM)) {
    Serial.println("Switch mid left hit");
  }

  if (!digitalRead(SWITCH_MID_R_NUM)) {
    Serial.println("Switch mid right hit");
  }

  if (!digitalRead(SWITCH_TB_NUM)) {
    Serial.println("Switch top or bottom hit");
  }
}

//Use to send an Array to Raspberry
/*
  static int data[3|={x,y,isHomed};
  -0 position X
  -1 postion Y
  -2 homed 0/1
  (-3 maybe use to tell raspberry that you are not into configuration mode) 
*/
void sendDatatoRaspberry()
{
  static int data[3] = {0, 0, 0};
  //static int temp_data[2]= {0,0};
  if (error == false)
  {
    if ((int)x != data[0] || (int)y != data[1])
    {
      data[0] = x;
      data[1] = y;

      if (homed == true)
      {
        data[2] = 1;
      } else {
        data[2] = 0;
      }

      Serial.println(String(data[0]) + "," + String(data[1]) + "," + String(data[2]));
    }

  } else {

    if (error_printed == false)
    {
      Serial.println("Error unplug Table wait 5sec and try again!");
      error_printed = true;

    }
  }
}


ISR(TIMER3_COMPA_vect)      //toggle Pul pins
{
  PORT_PUL_L ^= (1 << PIN_PUL_L);
  PORT_PUL_R ^= (1 << PIN_PUL_R);
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
  SteppermovetoXY(100, 100);
  //pushSolenoid();
  delay(700);
  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
}

void fans()
{
  if (fansOn == true)
  {
    digitalWrite(PIN_FANS, LOW);
    fansOn = false;
  } else
  {
    digitalWrite(PIN_FANS, HIGH);
    fansOn = true;
  }
}

void demo()
{
  for(int i=0;i<=1;i++)
  {
  SteppermovetoXY(100, 150);
  delay(100);
  SteppermovetoXY(450, 150);
  delay(100);
  SteppermovetoXY(450, 300);
  delay(100);
  SteppermovetoXY(100, 300);
  delay(100);
  }
  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
  fans();
  delay(2000);
  fans();
  goal();
  doHoming();
  Serial.println("Demo finished");
}

void cycleTest()  //Check all end switches (-#-> wait until button get hits, if button is damaged stay at postion to know which button is damaged)
{

  detachInterrupts();

  Serial.println("Cycle-Test start:");
  setDefaultParams();

  SteppermovetoXY(X_SWITCH_MID_L, DEFAULT_Y); //Move to left button
  delay(500);
  buttonPressed();

  SteppermovetoXY(X_SWITCH_MID_R, DEFAULT_Y);  //Move to right button
  delay(500);
  buttonPressed();

  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);  //Move to Default Position
  delay(250);

  SteppermovetoXY(DEFAULT_X, Y_SWITCH_BOTTOM); // Move to bottom button
  delay(500);
  buttonPressed();

  SteppermovetoXY(DEFAULT_X, Y_SWITCH_TOP);  //Move to top button
  delay(500);
  buttonPressed();
  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
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


