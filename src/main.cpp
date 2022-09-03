#include <Arduino.h>
#include "constants.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "uart.h"
#include "interrupts.h"
#include "home.h"
#include "updateSpeeds.h"


SoftwareSerial mySoftwareSerial(13, 10); // RX, TX


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
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  

//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB
//  }

   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }

  myDFPlayer.volume(25);
  myDFPlayer.play(1);
  myDFPlayer.stop();

  //init LDR
  pinMode(GOAL_ROBOT, INPUT);
  pinMode(GOAL_HUMAN, INPUT);
 
 
  // set end switches as input
  pinMode(SWITCH_SLIDER_2, INPUT_PULLUP);
  pinMode(SWITCH_MOTOR, INPUT_PULLUP);
  pinMode(SWITCH_OTHERS, INPUT_PULLUP);
  //pinMode(GOAL_SENSOR_ROBOT,INPUT);
  attachInterrupts();
  switch_slider = false; switch_others = false;

  //init Driver Fault Pins
  pinMode(DRIVER_FLT_L, INPUT);
  pinMode(DRIVER_FLT_R, INPUT);


  pinMode(PIN_FANS, OUTPUT);
  digitalWrite(PIN_FANS, LOW);

  pinMode(PIN_SOLENOID, OUTPUT);
  digitalWrite(PIN_SOLENOID, LOW);

  pinMode(PIN_LEDS,OUTPUT);
  digitalWrite(PIN_LEDS,HIGH);

  pinMode(PIN_PH_LED,OUTPUT);
  digitalWrite(PIN_PH_LED,LOW);  

  //init Stepper
   //set PUL1 and DIR1 to be output  --MOTOR1--
  DDRD |= (1 << PUL1);
  DDRD |= (1 << DIR1);
  //set PUL2 and DIR2 to be output  --MOTOR2--
  DDRC |= (1 << PUL2);
  DDRD |= (1 << DIR2);

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

  //TIMER 4 - high speed 10-bit timer - check goal and send data to Raspberry, see datasheet page 168
  //Set to prescaler of 512, Timer4 has frequency 64MHz
  //1 increment lasts = prescaler/64000000 us, one cycle lasts OCR4B_value*increment = 0.0008ms and thus 1250x times per second, sending data is RASPBERRY_DATA_LAG times less frequent
  TCCR4B |= (1 << CS43);
  TCCR4B &= ~(1 << CS42);
  TCCR4B |= (1 << CS41);
  TCCR4B &= ~(1 << CS40);
  TCNT4 = 0;  //set counter to 0
  OCR4B = OCR4B_value;
  TIMSK4 |= (1 << OCIE4B);   //allow timer4 interrupt


  sei();  //enable all interrupts
  setZeroSpeeds();
  delay(50);
  setDefaultParams();
  
  if (fansOn == false)
  { fans();
  }

  delay(1000); //wait for 2s --> driver fault pins toggle at beginning


}

void loop() {
  checkSerialInput();
  evaluatePos(pos_stepper[0], pos_stepper[1], pos_X, pos_Y);
  checkDriverError();


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


  
  if(goal_state == false)
  {
    checkGoal();
  }
  
//  if (homed == false)
//  {
//    homing();
//  }
}