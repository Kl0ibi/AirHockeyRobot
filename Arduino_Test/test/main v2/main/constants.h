#ifndef CONSTANTS_H
#define CONSTANTS_H

// Control mode
const bool control_mode = 1;

// Constant duration of loop()
unsigned long t = 0;
int BASE_CYCLE_DURATION = 2400;
const int FAST_CYCLE_DURATION = 0.65*BASE_CYCLE_DURATION;
int CYCLE_DURATION = BASE_CYCLE_DURATION;

const float START_Y = 150;
const float START_X = 300;

const int INDUCTION_CONSTANT_SWITCH = 2; //due to current in motors, there is inductive current in microswitch and thus we are searching for 2 consecutive events when switch stays constant and doesnt jump
const int INDUCTION_DRIVER_SWITCH = 3; //due to current in motors, there is inductive current in microswitch and thus we are searching for 2 consecutive events when switch stays constant and doesnt jump

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
bool state_switch_mid_l = false;
bool state_switch_mid_r = false;
bool state_switch_tb = false;

//Driver Fault Pins
#define DRIVER_FLT_L A2
#define DRIVER_FLT_R A3

//H-BOT CONSTANTS
const float mmPerRev = 2 * 60 * 0.5; //60 teeths *2mm teeth span
const float stepsPerRev = 400;
const float HBOT_CONSTANT = mmPerRev / stepsPerRev; //use to calc mmtosteps and stepstomm

//Barriers
const float OFFSET_X = 12; //X-Offset after homing
const float OFFSET_Y = 0; //Y-Offset after homing
const float PUSHER_RADIUS = 47.5;
//X MIN MAX
const float BARRIER_X_MIN = 0 + OFFSET_X + PUSHER_RADIUS;
const float BARRIER_X_MAX = 600 - OFFSET_X;
//Y MIN MAX
const float BARRIER_Y_MIN = 0 + OFFSET_Y + PUSHER_RADIUS;
const float BARRIER_Y_MAX = 421 - OFFSET_Y;
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
bool error = false;
bool homed = false;
bool error_drivers = false;
bool error_printed = false;
bool fansOn = false;
bool solenoidOn=false;
bool goal_state=true;


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

volatile bool realSpeedsApplied = false;

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

//Timer 4 OCR-value -->to use timer4 in highspeed mode u need to define OCR-value additionally
#define OCR4B_value 100

#endif
