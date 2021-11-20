#ifndef CONSTANTS_H
#define CONSTANTS_H

//Global Var
float x;
float y;
float ix=0;
float iy=0;



//Global CONSTANTS
const int DEBOUNCE_SWITCH=2;
const int INDUCTION_DRIVER=2;

//Global interrupt var
volatile uint16_t cntsteps=0;

//Fan Pins
#define PIN_FANS 8

// Stepper Pins
//Stepper DDR
#define DDR_PUL_L DDRD
#define DDR_DIR_L DDRD
#define DDR_PUL_R DDRC
#define DDR_DIR_R DDRD
//Stepper PORT
#define PORT_PUL_L PORTD
#define PORT_DIR_L PORTD
#define PORT_PUL_R PORTC
#define PORT_DIR_R PORTD
//Stepper PINS
#define PIN_PUL_L 6
#define PIN_DIR_L 4
#define PIN_PUL_R 6
#define PIN_DIR_R 7

// Switches
// Switch left middle
#define SWITCH_MID_L_NUM 7  
#define PIN_SWITCH_MID_L PINE
#define SWITCH_MID_L 6    //PE6
//Switch right middle
#define SWITCH_MID_R_NUM 3
#define PIN_SWITCH_MID_R PIND
#define SWITCH_MID_R 0    //PD0
//Switch top bottom
#define SWITCH_TB_NUM 2
#define PIN_SWITCH_TB PIND
#define SWITCH_TB 1       //PD1
//State interrupts
bool state_switch_mid_l=false;
bool state_switch_mid_r=false;
bool state_switch_tb=false;

//Driver Fault Pins
#define DRIVER_FLT_L A2
#define DRIVER_FLT_R A3

//H-BOT CONSTANTS
const float mmPerRev=2*60*0.5; //60 teeths *2mm teeth span
const float stepsPerRev=400;
const float HBOT_CONSTANT=mmPerRev/stepsPerRev; //use to calc mmtosteps and stepstomm

//Barriers
const float OFFSET_X=12; //X-Offset after homing
const float OFFSET_Y=0;  //Y-Offset after homing
const float PUSHER_RADIUS=47.5;
//X MIN MAX
const float BARRIER_X_MIN=0+OFFSET_X+PUSHER_RADIUS;
const float BARRIER_X_MAX=600-OFFSET_X;
//Y MIN MAX
const float BARRIER_Y_MIN=0+OFFSET_Y+PUSHER_RADIUS;
const float BARRIER_Y_MAX=421-OFFSET_Y;
//X Default
const float DEFAULT_X=300;    //Start X 
//Y Default
const float DEFAULT_Y=150;    //Start Y


//States
bool error=false;
bool homed=false;
bool error_drivers=false;
bool error_printed=false;
bool fansOn=false;

//Timer 4 OCR-value -->to use timer4 in highspeed mode u need to define OCR-value additionally
#define OCR4B_value 100

#endif
