#include "constants.h"

//calculate mm to steps
float mmToSteps(const float mm) {       
  return mm/HBOT_CONSTANT;
}

//calculate steps to mm
float stepsTomm(const float steps)  {   
  return steps*HBOT_CONSTANT;
}

void setup() 
{  
  //init uart0 with 115200 baud
  Serial.begin(115200);

  // init switches
  pinMode(SWITCH_MID_L_NUM, INPUT);
  pinMode(SWITCH_MID_R_NUM, INPUT);
  pinMode(SWITCH_TB_NUM, INPUT);

  //init Driver Fault Pins
  pinMode(DRIVER_FLT_L, INPUT);
  pinMode(DRIVER_FLT_R, INPUT);

  //init Stepper
  init_steppers();

  //init Timer
  //Timer3, CTC-mode, Prescaler 8
  TCCR3A=0;
  TCCR3B&=~(1<<WGM33);
  TCCR3B|=(1<<WGM32); 
  TCCR3B&=~(1<<CS30);                 
  //TCCR3B|=(1<<CS31);
  TCCR3B&=~(1<<CS31);
  TCCR3B&=~(1<<CS32);
  //TIMSK3|=(1<<OCIE3A);
  TIMSK3&=~(1<<OCIE3A);
  TCNT3=0;
  OCR3A=700;

  //Timer4, high speed 10bit timer, prescaler 512
  TCCR4B |= (1 << CS43);
  TCCR4B &= ~(1 << CS42);
  TCCR4B |= (1 << CS41);
  TCCR4B &= ~(1 << CS40);
  TCNT4 = 0; 
  OCR4B = OCR4B_value;    //to use timer4 in highspeed mode u need to define OCR-value additionally
  TIMSK4 |= (1 << OCIE4B); 

  sei();  //enable all interrupts
   
  delay(2000); //wait for 2s --> driver fault pins toggle at beginning
  
}

void loop() {
  checkSerialInput();
  checkDriverError();
  if(homed==false)
  {
    doHoming();
  }
}







