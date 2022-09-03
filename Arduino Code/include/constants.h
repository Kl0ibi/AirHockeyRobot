#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;

float mmToSteps(const float mm);

float stepsTomm(const float steps);

void evaluatePos(const volatile float _steps0, const volatile float _steps1, volatile float& _pos_X, volatile float& _pos_Y);

const bool control_mode = 1;  // 1 is P regulator, 0 is s-curve mode

// Constant duration of loop()
unsigned long t = 0;
int BASE_CYCLE_DURATION = 2400;
const int FAST_CYCLE_DURATION = 0.65*BASE_CYCLE_DURATION;
int CYCLE_DURATION = BASE_CYCLE_DURATION;



//Driver fault
#define DRIVER_FLT_0 A2
#define DRIVER_FLT_1 A3

//Global Var
float x;
float y;
float ix = 0;
float iy = 0;


//Global CONSTANTS
const int DEBOUNCE_SWITCH = 2;
const int INDUCTION_DRIVER = 2;

//Global interrupt var
volatile uint16_t cntsteps = 0;
volatile bool detachSwitchTB = true;
volatile bool detachSwitchmidL = true;
volatile bool detachSwitchmidR = true;


//LDR Pins / Goal
#define GOAL_ROBOT 4
#define GOAL_HUMAN 5


//Fan Pins
#define PIN_FANS 9
int fan_state = 0;

//Led Pins
#define PIN_LEDS 1
#define PIN_PH_LED 0

bool blinking = false;
bool blinking_state = false;
#define BLINK_COUNT_MAX 16
#define BLINK_NR 20

//Solenoid Pins
#define PIN_SOLENOID 11



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

// End switches
#define SWITCH_SLIDER_2 7 //PE6
#define SWITCH_SLIDER_2_PIN PINE
#define SWITCH_SLIDER_2_REGISTER_NUM 6

#define SWITCH_MOTOR 2 //PD0
#define SWITCH_MOTOR_PIN PIND
#define SWITCH_MOTOR_REGISTER_NUM 1

#define SWITCH_OTHERS 3  //PD1
#define SWITCH_OTHERS_PIN PIND
#define SWITCH_OTHERS_REGISTER_NUM 0  
//State interrupts
bool state_switch_mid_l = false;
bool state_switch_mid_r = false;
bool state_switch_tb = false;

//switch interrupts
volatile bool switch_slider = false;
volatile bool switch_motor = false;
volatile bool switch_others = false;
volatile bool realSpeedsApplied = false;

// Steppers pins
const int PUL1 = PD6; // Pul pin of stepper 1, Digital PIN 12
const int DIR1 = PD4; // cDir pin of stepper 1, Digital PIN 4
const int PUL2 = PC6; // Pul pin of stepper 2, Digital PIN 5
const int DIR2 = PD7; // Dir pin of stepper 2, Digital PIN 6

//Driver Fault Pins
#define DRIVER_FLT_L A2
#define DRIVER_FLT_R A3

//H-BOT CONSTANTS
// Hbot contruction
const float mmPerRev = 0.5 * 60 * 2; //60 teeth * 2mm teeth span
const float stepsPerRev =  400; //microstepping
const float HBOT_CONSTANT = mmPerRev/stepsPerRev; //mm->steps means divide, steps->mm means multiply (=0,15)


// Speed variables
float ACCEL_PER1SEC;  //accel for axis per 1 second
float DECEL_PER1SEC;  //accel for axis per 1 second
float DECEL;
float ACCEL;  //accel for motor in steps per cycle
float MM_SPEED;
float MAX_MOTOR_SPEED;
unsigned int Kp;

// Default values
const unsigned long ACCEL_PER1SEC_DEF = 23000; //acceleration per 1 second
const unsigned long DECEL_PER1SEC_DEF = 30000; //deceleration per 1 second
const int MM_SPEED_DEF = 6000; //mm per second
const int Kp_DEF = 20; //P regulator

// Boundaries
#define MAX_ALLOWED_ACCEL 200
#define MAX_ALLOWED_ACCEL_PER1SEC 200000
#define MAX_MOTOR_ALLOWED_SPEED 20000
#define MAX_KPGAIN 1000


const unsigned int SPEED_TO_UPDATE = 0.3; //speed lower than this difference is ignored and not updated
const float POSITION_ERROR_TOLERANCE = 1.5; //must be greater than 0!!!
const unsigned int FLUCTUATION_DEVIATION = 4;

//Barriers
// Barriers
const uint8_t OFFSET_X = 15;
const byte OFFSET_Y = 11;
const float PUSHER_RADIUS = 47.5;
const float BARRIER_X_MIN  = 0 + PUSHER_RADIUS + OFFSET_X;
const float BARRIER_X_MAX = 425;
const float BARRIER_Y_MIN  = -300 + PUSHER_RADIUS + OFFSET_Y;
const float BARRIER_Y_MAX = 300 - PUSHER_RADIUS - OFFSET_Y;
const float START_Y = BARRIER_Y_MAX;
const float START_X = BARRIER_X_MIN;


const int INDUCTION_CONSTANT_SWITCH = 2; //due to current in motors, there is inductive current in microswitch and thus we are searching for 2 consecutive events when switch stays constant and doesnt jump
const int INDUCTION_DRIVER_SWITCH = 3; //due to current in motors, there is inductive current in microswitch and thus we are searching for 2 consecutive events when switch stays constant and doesnt jump
//X Default
const float DEFAULT_X = 300;  //Start X
//Y Default
const float DEFAULT_Y = 150;  //Start Y
//Switch left
const float X_SWITCH_MID_L = 60;
//Switch right
const float X_SWITCH_MID_R = 550;
//Switch bottom
const float Y_SWITCH_BOTTOM = 50;
//Switch top
const float Y_SWITCH_TOP = 400;


//States
bool error = true;
bool error_drivers = false;
bool last_error_driver = false;
bool homing_state = false;
bool error_printed = false;
bool homed = false;
bool fansOn = false;
bool solenoidOn=false;
bool goal_state=true;
bool led_state=true;

//Timer 4 OCR-value -->to use timer4 in highspeed mode u need to define OCR-value additionally
#define OCR4B_value 100

// Timer Interrupt Compare variables
#define TIMER_LIMIT 65535
const int prescaler = 8; // if you change this value, you need to change TCCR1B and TCCR3B too!!!
const long COMP_CONSTANT = 16000000/prescaler;
volatile int Tim1_count = 0;  //counter to go above highest value to compare
int Tim1_multiplier = 0;  //used to compare Tim1_count if minimal speed is too low for one comp cycle
uint16_t Tim1_res_comp = 0;
volatile int Tim3_count = 0;
int Tim3_multiplier = 0;
uint16_t Tim3_res_comp = 0;

#define MEM_SIZE 8
unsigned long signal_timestamp[MEM_SIZE];


//Timer4 constants
#define RASPBERRY_DATA_LAG 8


//Position
volatile float pos_stepper[2] = {0,0};
volatile float pos_X = 0;
volatile float pos_Y = 0;
volatile float pos_braking_X = 0;
volatile float pos_braking_Y = 0;
const float BRAKING_OFFSET = 300;
bool caution_braking = false;

// Speed variables
bool preventWallHit = true;
bool preventWallHit_printed = false;
bool allowedSpeed[2] = {false,false};
bool changed[2] = {false,false};
const int initial_speed = 0; 
const unsigned int minimal_speed = 1; //minimal speed: 31 steps/sec for 8 prescaler
bool positionControl = false;
bool positionReached = false;
float desiredPosition[2] = {START_X,START_Y};
float desiredSpeed[2] = {0,0};
float _desiredSpeedToPass[2];
float desiredAccel[2];
float desiredBrakingAccel[2];
float realSpeedXY_mm[2] = {0,0};
float speedToCompare[2] = {0,0};
float realSpeed[2] = {0,0};
int direct[2] = {0, 0}; //1 = CCW, -1 = CW
int dir_state[2];

#endif