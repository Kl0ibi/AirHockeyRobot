//#include "constants.h"

#define resetswitch()    state_switch_tb = false;state_switch_mid_l = false;state_switch_mid_r = false

void attachInterrupts()  {
  attachInterrupt(digitalPinToInterrupt(SWITCH_SLIDER_2), checkSwitchSlider2, LOW);
  attachInterrupt(digitalPinToInterrupt(SWITCH_MOTOR), checkSwitchMotor, LOW);
  attachInterrupt(digitalPinToInterrupt(SWITCH_OTHERS), checkSwitchOthers, LOW);
}

void detachInterrupts() {
  detachInterrupt(digitalPinToInterrupt(SWITCH_SLIDER_2));
  detachInterrupt(digitalPinToInterrupt(SWITCH_MOTOR));
  detachInterrupt(digitalPinToInterrupt(SWITCH_OTHERS));
}

void errorTrigger()  {
  error = true;
  error_printed = false;
  if (!homing_state)  {
    homed = false;
  }
  //Serial.println("ERROR TRIGGER");
  //print_pos();
  setZeroSpeeds();
  detachInterrupts();
}

void checkDriverError() {  
  // check if error has occured
  if ((!digitalRead(DRIVER_FLT_0)) || (!digitalRead(DRIVER_FLT_1))) {
    //Serial.println("going to check drivers");
    if (!error_drivers) {
      int i = 0;
      while (!(digitalRead(DRIVER_FLT_0)) || !(digitalRead(DRIVER_FLT_1))) {  // && !error_drivers
        if (++i >= INDUCTION_DRIVER_SWITCH)  {
          if (!last_error_driver) {
            error_drivers = true;
            errorTrigger();
            return;
          }
        }
      }
    }
  }
  // check if error has been dismissed
  else {
    if (digitalRead(DRIVER_FLT_0) && digitalRead(DRIVER_FLT_1)) 
      error_drivers = false;
  }
}


void checkSwitchSlider2()  {
  int i = 0;
  while (!bitRead(SWITCH_SLIDER_2_PIN,SWITCH_SLIDER_2_REGISTER_NUM) && !switch_slider ) {
    //Serial.println("slider");
    if (++i >= INDUCTION_CONSTANT_SWITCH)  {
      //Serial.println("slider2 switch");
      switch_slider = true;
      errorTrigger();
      return;
    }
  }
}

void checkSwitchMotor()  {
  int i = 0;
  while (!bitRead(SWITCH_MOTOR_PIN,SWITCH_MOTOR_REGISTER_NUM) && !switch_motor) {
    Serial.println("motor");
    if (++i >= INDUCTION_CONSTANT_SWITCH)  {
      //Serial.println("motor switch");
      switch_motor = true;
      errorTrigger();
      return;
    }
  }
}

void checkSwitchOthers()  {
  int i = 0;
  //Serial.println("checkSwitchOthers, value = " +String(bitRead(SWITCH_OTHERS_PIN,SWITCH_OTHERS_REGISTER_NUM)));
  while (!bitRead(SWITCH_OTHERS_PIN,SWITCH_OTHERS_REGISTER_NUM) && !switch_others ) {
    //Serial.println("others i = " + String(i));
    if (++i >= INDUCTION_CONSTANT_SWITCH)  {
      //Serial.println("others switch");
      switch_others = true;
      errorTrigger();
      return;
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


void checkGoal()  {
 int LDR_refrence=900;
 int LDR_HUMAN; 
 int LDR_ROBOT;
 float maxspeed_temp;
 
 LDR_HUMAN=analogRead(GOAL_HUMAN);
 LDR_ROBOT=analogRead(GOAL_ROBOT);
 
 //Serial.println(LDR_HUMAN);
 
 if(LDR_HUMAN<=LDR_refrence)
 {
  goal_state=true;
  blinking=true;
     TCCR1B &= ~(1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  //Set to prescaler of 8
  TCCR3B &= ~(1 << CS12);
  TCCR3B &= ~(1 << CS11);
  TCCR3B &= ~(1 << CS10);
  
    //ledBlink();
    setZeroSpeeds();
//    for (int i =0;i<800;i++){
//      //  MAX_MOTOR_SPEED=500;
//        loopWithoutSerial();
//       // waitUntilPosReached(200,155);
//      }
    delay(100);
     maxspeed_temp=MAX_MOTOR_SPEED;
    setDesiredPosition(200,-200);
TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS10);
  //Set to prescaler of 8
  TCCR3B &= ~(1 << CS12);
  TCCR3B |= (1 << CS11);
  TCCR3B &= ~(1 << CS10);
  //Serial.println("gr");
  //waitUntilPosReached(200,155);
    
    
  
  for (int i =0;i<400;i++){
        //MAX_MOTOR_SPEED=500;
        loopWithoutSerial();
        //waitUntilPosReached(200,155);
        setDesiredPosition(200,-200);
         if(i==200)
        {
          Serial.println("gr");
           pushSolenoid();
           setZeroSpeeds();
        }
     }
     delay(200);
     goal_state=false;
      //MAX_MOTOR_SPEED=maxspeed_temp;
    //waitUntilPosReached(100,-200);
  //delay(2000);
//  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);

 
   //goal_state=true;
  //setDesiredPosition(200,155);
   //setZeroSpeeds();
   //setZeroSpeeds();
  //ledBlink();
  //maxspeed_temp=MAX_MOTOR_SPEED;
   
  // Serial.println("gr");
  
  //for (int i =0;i<1500;i++){
      //  MAX_MOTOR_SPEED=500;
        //loopWithoutSerial();
       // waitUntilPosReached(200,155);
      //}
     // MAX_MOTOR_SPEED=maxspeed_temp;
//  playGoalWinningSound();
    
//  //goal();
    //waitUntilPosReached(100,-200);
    
    
//  playGameSound();
 }

 if(LDR_ROBOT<=LDR_refrence)
 {
  goal_state=true;
  blinking=true;
   // goal_state=true;
//  playGoalLosingSound();


     TCCR1B &= ~(1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  //Set to prescaler of 8
  TCCR3B &= ~(1 << CS12);
  TCCR3B &= ~(1 << CS11);
  TCCR3B &= ~(1 << CS10);
  
    //ledBlink();
    setZeroSpeeds();
//    for (int i =0;i<800;i++){
//      //  MAX_MOTOR_SPEED=500;
//        loopWithoutSerial();
//       // waitUntilPosReached(200,155);
//      }
    delay(100);
    Serial.println("gh");
     maxspeed_temp=MAX_MOTOR_SPEED;
    setDesiredPosition(200,-200);
TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS10);
  //Set to prescaler of 8
  TCCR3B &= ~(1 << CS12);
  TCCR3B |= (1 << CS11);
  TCCR3B &= ~(1 << CS10);
  
  //waitUntilPosReached(200,155);

for (int i =0;i<400;i++){
        //MAX_MOTOR_SPEED=500;
        loopWithoutSerial();
        //waitUntilPosReached(200,155);
        setDesiredPosition(200,-200);
       
     }
  
  //for (int i =0;i<1500;i++){
        //MAX_MOTOR_SPEED=500;
        //loopWithoutSerial();
        
        //setDesiredPosition(200,155);
     // }
      //MAX_MOTOR_SPEED=maxspeed_temp;
    //waitUntilPosReached(100,-200);
    delay(200);
//  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
  goal_state=false;
 }

 
}


//void checkGoal()  {
// int LDR_refrence=900;
// int LDR_HUMAN; 
// int LDR_ROBOT;
// float maxspeed_temp;
// 
// LDR_HUMAN=analogRead(GOAL_HUMAN);
// LDR_ROBOT=analogRead(GOAL_ROBOT);
// 
// //Serial.println(LDR_HUMAN);
// 
// if(LDR_HUMAN<=LDR_refrence)
// {
//     goal_state=true;
//     Serial.println("gr");
//     setDesiredPosition(200,155);
//     
//     pushSolenoid();
//     delay(100);
//     setZeroSpeeds();
//     goal_state=false;
// }
//
// if(LDR_ROBOT<=LDR_refrence)
// {
//     goal_state=true;
//     Serial.println("gh");
//     setZeroSpeeds();
//     goal_state=false;
// }
//
// 
//}

//Use to send an Array to Raspberry
/*
  static int data[3|={x,y,isHomed};
  -0 position X
  -1 postion Y
  -2 homed 0/1
  (-3 maybe use to tell raspberry that you are not into configuration mode) 
*/
void sendDataToRaspberry()  { //Timer for sending serial data
  static int sent[5] = {0,0,0,0,-1};
  if (error && !error_printed && !homing_state)  {
    if (error_drivers) {
      Serial.println("e2");
      last_error_driver = true;
    }
    else {
      Serial.println("e1");
    }
    //printTimestamps0();
    error_printed = true;
  }
  if ((int)pos_X != sent[0] || (int)pos_Y != sent[1] || (int)realSpeedXY_mm[0] != sent[2] || (int)realSpeedXY_mm[1] != sent[3]) {
    sent[0] = (int)pos_X; sent[1] = (int)pos_Y; sent[2] = (int)realSpeedXY_mm[0]; sent[3] = (int)realSpeedXY_mm[1];
    if (!homed)  {
      sent[4] = (int)homed; 
      Serial.println(String((sent[0])) + "," + String(sent[1]) + ";" + String(sent[2]) + "," + String(sent[3]) + ";" + String(sent[4]));
    }
    else  {
      Serial.println(String((sent[0])) + "," + String(sent[1]) + ";" + String(sent[2]) + "," + String(sent[3]));
    }
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
    PORTC|=(1<<PUL2);
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
    PORTC&=(~(1<<PUL2));
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
    PORTD|=(1<<PUL1);
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
    PORTD&=(~(1<<PUL1));
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
  static int i = 0;
    static int blink_count = 0;
  static int blink_nr = 0;
  interrupts(); 
  if (++i >= RASPBERRY_DATA_LAG) {
      sendDataToRaspberry();
    i = 0;
  }
  if (blinking) {
    if (++blink_count == BLINK_COUNT_MAX)  {
      blinking_state = !blinking_state;
      digitalWrite(PIN_LEDS,blinking_state);
      //digitalWrite(PIN_LEDS,HIGH);
      blink_count=0;
      if (++blink_nr == BLINK_NR) {
        blinking = false;
        digitalWrite(PIN_LEDS,!blinking_state);
        blink_nr=0;
      }
    }
  }
   TCNT4=0;
}


void goal()
{
  ledBlink();
  //playGoalSound();
  
  waitXY(150, 100);
  pushSolenoid();
  delay(700);
  waitXY(300, 150);
  goal_state=false;
  delay(10);
  
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

void manipulateFan(int state) {
  fan_state = state;
  if (state)  {
    digitalWrite(PIN_FANS, LOW);
  }
  else  {
    digitalWrite(PIN_FANS, HIGH);
  }
}

void demo()
{
  for(int i=0;i<=1;i++)
  {
  setDesiredPosition(100, 150);
  delay(100);
  setDesiredPosition(450, 150);
  delay(100);
  setDesiredPosition(450, 300);
  delay(100);
  setDesiredPosition(100, 300);
  delay(100);
  }
  setDesiredPosition(DEFAULT_X, DEFAULT_Y);
  fans();
  delay(2000);
  fans();
  goal();
  delay(100);
  homing();
  Serial.println("Demo finished");
}

void cycleTest()  //Check all end switches (-#-> wait until button get hits, if button is damaged stay at postion to know which button is damaged)
{

//  detachInterrupts();
//
//  Serial.println("Cycle-Test start:");
//  setDefaultParams();
//
//  SteppermovetoXY(X_SWITCH_MID_L, DEFAULT_Y); //Move to left button
//  delay(500);
//  buttonPressed();
//
//  SteppermovetoXY(X_SWITCH_MID_R, DEFAULT_Y);  //Move to right button
//  delay(500);
//  buttonPressed();
//
//  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);  //Move to Default Position
//  delay(250);
//
//  SteppermovetoXY(DEFAULT_X, Y_SWITCH_BOTTOM); // Move to bottom button
//  delay(500);
//  buttonPressed();
//
//  SteppermovetoXY(DEFAULT_X, Y_SWITCH_TOP);  //Move to top button
//  delay(500);
//  buttonPressed();
//  SteppermovetoXY(DEFAULT_X, DEFAULT_Y);
}


void pushSolenoid()
{

    digitalWrite(PIN_SOLENOID, HIGH);
    delay(1000);
    digitalWrite(PIN_SOLENOID, LOW); 
  
}

void ledBlink()
{
   if(led_state==false)
  {
    digitalWrite(PIN_LEDS,HIGH);
    
  }

  for(int i=0; i<=5;i++)
  {
  digitalWrite(PIN_LEDS,LOW);
  delay(50);
  digitalWrite(PIN_LEDS,HIGH);
  delay(50);
  }

  led_state=true;
 
}

void leds(int value)
{
  if(value<=10)
  {
    digitalWrite(PIN_LEDS,LOW);
    led_state=false;
  }else if (value>10){
     digitalWrite(PIN_LEDS,HIGH);
    led_state=true;
  }
}

void playGoalLosingSound()
{
  myDFPlayer.play(4);
}

void playGoalWinningSound()
{
  myDFPlayer.play(3);
}

void playStartup()
{
  myDFPlayer.play(1);
}

void puckNotDetected()
{
  digitalWrite(PIN_PH_LED,HIGH);  
}

void puckDetected()
{
  digitalWrite(PIN_PH_LED,LOW);  
}

void playGameSound()
{
  myDFPlayer.play(2);
}
