#ifndef UPDATESPEEDS_H
#define UPDATESPEEDS_H

#include <Arduino.h>
#include "constants.h"

bool stepsToComp(float steps, int& Tim_multiplier, uint16_t& Tim_res_comp);

void resetDirections();

void setDesiredSpeedsXY(float v_x, float v_y);

void updateRealSpeedXY_mm();

void setDesiredSpeedsMotors(float v0, float v1);

void setZeroSpeeds();

void waitXY(float x, float y);

void setXY(float x, float y);

void waitUntilPosReached(float x, float y);

void setDesiredPosition(float x, float y);

void resetDesiredPosition();

void resetPosition();

float mapf(float val, float in_min, float in_max, float out_min, float out_max);

bool oppositeSignsAdvanced(float x, float y);

bool oppositeSigns(float x, float y);

void P_regulator();

void analytic_control();

void setDefaultParams();

bool isMotorDecelerating(float v_desired, float v_real);

float pickAccel(float v_desired, float v_real);

void scaleToMaxMotorValues(float v0_diff, float v1_diff, float max_value_0, float max_value_1, float& desiredValue0, float& desiredValue1);

void updateRealSpeeds();

void applyRealSpeeds();

void applyRealSpeed0();

void applyRealSpeed1();




#endif 