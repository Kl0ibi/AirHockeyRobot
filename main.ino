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

  //init Timer
  //Timer3, CTC-mode, Prescaler 8
  TCCR3A = 0;
  TCCR3B &= ~(1 << WGM33);
  TCCR3B |= (1 << WGM32);
  TCCR3B &= ~(1 << CS30);
  //TCCR3B|=(1<<CS31);
  TCCR3B &= ~(1 << CS31);
  TCCR3B &= ~(1 << CS32);
  //TIMSK3|=(1<<OCIE3A);
  TIMSK3 &= ~(1 << OCIE3A);
  TCNT3 = 0;
  OCR3A = 700;

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
  }
}







