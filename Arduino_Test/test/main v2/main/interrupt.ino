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

void checkGoal()  {
 int LDR_refrence=900;
 int LDR_HUMAN; 
 int LDR_ROBOT;
 
 LDR_HUMAN=analogRead(GOAL_HUMAN);
 //LDR_ROBOT=analogRead(GOAL_ROBOT);
 
 //Serial.println(LDR_HUMAN);
 
 if(LDR_HUMAN<=LDR_refrence)
 {
  goal_state=true;
  Serial.println("gh");
  //playGoalWinningSound();
  //ledblink();
  goal();
 }

// if(LDR_ROBOT<=LDR_refrence)
// {
//  Serial.println("gr");
//  //playGoalLosingSound();
//  //ledblink();
//  delay(100);
//  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
// }

 
}



void setAccel(float _accel_per1sec) {
  //_accel_per1sec/=2.0;  //_accel_per1sec is acceleration for motor, not axis!!!!
  if (_accel_per1sec > 0) {
      ACCEL_PER1SEC = _accel_per1sec;
      //Serial.println("ACCEL_PER1SEC = " + String(ACCEL_PER1SEC));
      _accel_per1sec*=0.5;  //h-bot construction;
      ACCEL = mmToSteps((float)((_accel_per1sec*CYCLE_DURATION)/1000000.0));
      Serial.print("Setting accelertion = ");
      Serial.println(ACCEL);
  }
  else 
    Serial.println("Acceleration must be greater than 0 and lower than " + String(MAX_ALLOWED_ACCEL_PER1SEC));
}

void setDecel(float _decel_per1sec) {
  if (_decel_per1sec > 0) {
      DECEL_PER1SEC = _decel_per1sec;
      _decel_per1sec*=0.5;  //h-bot construction;
      DECEL = mmToSteps((float)((_decel_per1sec*CYCLE_DURATION)/1000000.0));
      Serial.print("Setting decelertion = ");
      Serial.println(DECEL);
  }
  else 
    Serial.println("Deceleration must be greater than 0");
}

//void _blink(int _value) {
//  Serial.println("Blinking");
//  for(int i = 0; i<9;i++) {
//    digitalWrite(GOALLED_STRIP,HIGH);
//    delay(50);
//    digitalWrite(GOALLED_STRIP,LOW);
//    delay(50);
//  }
//  Serial.println("Finished blinking");
//}

void setMaximalSpeed(float _maxspeed) {
  MM_SPEED = _maxspeed;
  _maxspeed = mmToSteps(_maxspeed);
  //Serial.println(_maxspeed);
  _maxspeed/=2; //max speed , XY coordinates is 2 times larger than max speed of motor
  if (_maxspeed > 0) {
    if (_maxspeed > MAX_MOTOR_ALLOWED_SPEED) {
      MAX_MOTOR_SPEED = MAX_MOTOR_ALLOWED_SPEED;
    }
    else {
      MAX_MOTOR_SPEED = _maxspeed;
    }
    //Serial.print("Setting maximal speed = ");
    //Serial.println(MAX_MOTOR_SPEED);
  }
  else 
    Serial.println("Max speed must be greater than 0 and lower than " + String(MAX_MOTOR_ALLOWED_SPEED));
}

void setKpGain(unsigned int _Kp) {
  if (_Kp > 0 && _Kp < MAX_KPGAIN)
    Kp = _Kp;
}

/*------------------------------------------------------------------------------------------------*/

void print_pos()  {
  Serial.print("Position X,Y: ");
  Serial.println(String(pos_X) + " " + String(pos_Y));
}

void print_desired_pos()  {
  Serial.print("Desired position X,Y: ");
  Serial.println(String(desiredPosition[0]) + " " + String(desiredPosition[1]));
}

void print_steps()  {
  Serial.print("Steppers steps 1,2: ");
  Serial.println(String(pos_stepper[0]) + " " + String(pos_stepper[1]));
}

void print_real_speeds()  {
  Serial.print("Real speeds: ");
  Serial.println(String(realSpeed[0]) + " " + String(realSpeed[1]));
}

void print_real_speedsXY()  {
  Serial.print("Real speeds XY: ");
  Serial.println(String(realSpeedXY_mm[0]) + " " + String(realSpeedXY_mm[1]));
}

void print_desired_speeds()  {
  Serial.print("Desired speeds: ");
  Serial.println(String(desiredSpeed[0]) + " " + String(desiredSpeed[1]));
}

void print_accels() {
  Serial.print("Accels: ");
  Serial.println(String(desiredAccel[0]) + " " + String(desiredAccel[1]));
}

void print_error()  {
  Serial.print("Error=");
  Serial.println(error);
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


void recordTimestamp0() {
  for(int i = MEM_SIZE - 1 ; i >= 1; i--) {
      signal_timestamp[i] = signal_timestamp[i-1];
  }
  signal_timestamp[0] = micros();
}

void printTimestamps0() {
  Serial.println("LAST TIMESTAMPS");
  for(int i = 0; i < MEM_SIZE; i++) {
    Serial.println("_" + String(signal_timestamp[i]) + "_");
  } 
}



ISR(TIMER1_COMPA_vect)  { //Timer for motor 1
  //Serial.println("TIMER1_COMPA_vect, OCR1A = "+ String(OCR1A) +" Tim1_count = " + String(Tim1_count) + " Tim1_multiplier = " + String(Tim1_multiplier));
  if (direct[1] == 0 || error)
    return;

  if(Tim1_count >= Tim1_multiplier)  {
    /*if (changing_dir[1])  {
      Serial.println("motor 1 changing direction iterrupted by doing step");
    }*/
    PORT_PUL_R |=(1<<PIN_PUL_R);
    pos_stepper[1] += direct[1];
    __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop");   // Wait for step pulse
    //CLR(PORTC, PUL2); CLR(x,y) (x&=(~(1<<y))) 
    PORT_PUL_R &=(~(1<<PIN_PUL_R));
    //Serial.println("motor 1 step");
    Tim1_count = 0;
    OCR1A = (Tim1_multiplier == 0)? (Tim1_res_comp) : (65535);
    return;
  }

  else {
    if(++Tim1_count == Tim1_multiplier)
      OCR1A = Tim1_res_comp;
      return;
  }
  
}

ISR(TIMER3_COMPA_vect)  { //Timer for motor 0
  if (direct[0] == 0 || error)
    return;
  
  if(Tim3_count >= Tim3_multiplier)  {
    /*if (changing_dir[0])  {
      Serial.println("motor 0 changing direction iterrupted by doing step");
    }*/
    PORT_PUL_L|=(1<<PIN_PUL_L);
    pos_stepper[0] += direct[0];
    __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop");  // Wait for step pulse
    //CLR(PORTD, PUL1); 
    //Serial.println("motor 0 step");
    PORT_PUL_L&=(~(1<<PIN_PUL_L));
    Tim3_count = 0;
    OCR3A = (Tim3_multiplier == 0)? (Tim3_res_comp) : (65535);
    //recordTimestamp0();
    return;
  }
  
  else {
    //Tim3_count++;
    if(++Tim3_count == Tim3_multiplier)   { 
      OCR3A = Tim3_res_comp;     
    } 
  }
}

//64MHz timer use to check for goal or errrors
ISR(TIMER4_COMPB_vect)
{
  sendDatatoRaspberry();
}


void goal()
{
//  //ledblink();
//  //playGoalSound();
  delay(1500);
  SteppermovetoXY(100, 100);
  pushSolenoid();
  delay(700);
  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
  goal_state=false;
  delay(1000);
  
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
  delay(100);
  doHoming();
  Serial.println("Demo finished");
}

void cycleTest()  //Check all end switches (-#-> wait until button get hits, if button is damaged stay at postion to know which button is damaged)
{

  detachInterrupts();

  Serial.println("Cycle-Test start:");
  setDefaultParams1();

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

    digitalWrite(PIN_SOLENOID, HIGH);
    delay(1000);
    digitalWrite(PIN_SOLENOID, LOW); 
  
}

void ledBlink()
{

}

void leds()
{

}
