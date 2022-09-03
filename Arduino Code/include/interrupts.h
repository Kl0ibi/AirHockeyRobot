#ifndef INTERRUPT_H
#define INTERRUPT_H

void attachInterrupts();

void detachInterrupts();

void errorTrigger();

void checkDriverError();

void checkSwitchSlider2();

void checkSwitchMotor();

void checkSwitchOthers();

void recordTimestamp0();

void printTimestamps0();

void checkGoal();

void sendDataToRaspberry();

void goal();

void fans();

void manipulateFan(int state);

void demo();

void cycleTest();

void pushSolenoid();

void ledBlink();

void leds(int value);

void playGoalLosingSound();

void playGoalWinningSound();

void playStartup();

void puckNotDetected();

void puckDetected();

void playGameSound();


#endif