#ifndef STEPPER_H
#define STEPPER_H


void init_steppers();

void changeDirection_L();

void changeDirection_R();

void changeDirection0();

void changeDirection1();

void Steppermoveleft();

void Steppermoveright();

void Steppermoveforward();

void Steppermovebackward();

void Stepperstop();

void setDesiredSpeed(int value);

void SteppermovetoXY(float _x, float _y);



#endif