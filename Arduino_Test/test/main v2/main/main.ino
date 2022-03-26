#include "constants.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


//SoftwareSerial mySoftwareSerial(10, 11); // RX, TX

//DFRobotDFPlayerMini myDFPlayer;

//calculate mm to steps
float mmToSteps(const float mm) {
  return mm / HBOT_CONSTANT;
}

//calculate steps to mm
float stepsTomm(const float steps)  {
  return steps * HBOT_CONSTANT;
}

void evaluatePos(const volatile float _steps0, const volatile float _steps1, volatile float& _pos_X, volatile float& _pos_Y)  {
  _pos_X = START_X + stepsTomm(-_steps0 + _steps1);
  _pos_Y = START_Y + stepsTomm(-_steps0 - _steps1);
}

void setup()
{
  //init uart0 with 115200 baud
  Serial.begin(115200);
//mySoftwareSerial.begin(9600);

//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB
//  }

//   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
//  }

  //myDFPlayer.volume(25);
  //myDFPlayer.play(1);

  //init LDR
  //pinMode(GOAL_ROBOT, INPUT);
  pinMode(GOAL_HUMAN, INPUT);
 
 
  // init switches
  pinMode(SWITCH_MID_L_NUM, INPUT);
  pinMode(SWITCH_MID_R_NUM, INPUT);
  pinMode(SWITCH_TB_NUM, INPUT);

  //init Driver Fault Pins
  pinMode(DRIVER_FLT_L, INPUT);
  pinMode(DRIVER_FLT_R, INPUT);


  pinMode(PIN_FANS, OUTPUT);
  digitalWrite(PIN_FANS, LOW);

   pinMode(PIN_SOLENOID, OUTPUT);
  digitalWrite(PIN_SOLENOID, LOW);
    
 

  //init Stepper
  init_steppers();

  resetDirections(); resetDirections();

  
  //init Timer
  //Reset Timer1 Control Reg A
  TCCR1A = 0;
  //Reset Timer3 Control Reg A
  TCCR3A = 0;

  // Set CTC mode on Timer1, it ensures that each time comp triggers, it automatically resets the timer to 0
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);
  // Set CTC mode on Timer3, it ensures that each time comp triggers, it automatically resets the timer to 0
  TCCR3B &= ~(1 << WGM13);
  TCCR3B |= (1 << WGM12);
  
  //Set to prescaler of 8
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
  TCCR1B &= ~(1 << CS10);
  //Set to prescaler of 8
  TCCR3B &= ~(1 << CS12);
  TCCR3B |= (1 << CS11);
  TCCR3B &= ~(1 << CS10);

  //Reset Timer1 on Motor 2 and set compare value
  TCNT1 = 0;
  //OCR1A = stepsToComp(initial_speed);
  //Reset Timer3 on Motor 1 and set compare value
  TCNT3 = 0;
  //OCR3A = stepsToComp(initial_speed);

  //Enable Timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);
  //Enable Timer3 compare interrupt
  TIMSK3 = (1 << OCIE3A);

  //Timer4, high speed 10bit timer, prescaler 512
  TCCR4B |= (1 << CS43);
  TCCR4B &= ~(1 << CS42);
  TCCR4B |= (1 << CS41);
  TCCR4B &= ~(1 << CS40);
  TCNT4 = 0;
  OCR4B = OCR4B_value;    //to use timer4 in highspeed mode u need to define OCR-value additionally
  TIMSK4 |= (1 << OCIE4B);


  sei();  //enable all interrupts

  attachInterrupts();
  setZeroSpeeds();
  delay(50);
  setDefaultParams();
  
  delay(2000); //wait for 2s --> driver fault pins toggle at beginning


}

void loop() {
  checkSerialInput();
  checkDriverError();
  
  if(goal_state == false)
  {
    checkGoal();
  }
  
  if (homed == false)
  {
    doHoming();
  }else{

  if (!error) {
    updateRealSpeeds();
  } 
  if (micros()-t >= CYCLE_DURATION)  {
    Serial.println("Loop took more: " + String(micros()-t));
  }
  else {
    while(micros()-t < CYCLE_DURATION){}  //wait for cycle to be time-equidistant 
  }
  applyRealSpeeds();
  t = micros();
  }
}
