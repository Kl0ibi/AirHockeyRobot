#ifndef UART_H
#define UART_H

void test_single(float accel, unsigned int max_speed, double rad, double start_Y);

void checkSerialInput();

void setAccel(float _accel_per1sec);

void setDecel(float _decel_per1sec);

void setMaximalSpeed(float _maxspeed);

void setKpGain(unsigned int _Kp);

void print_pos();

void print_desired_pos();

void print_steps();

void print_real_speeds();

void print_real_speedsXY();

void print_desired_speeds();

void print_accels();

void print_error();

int readline(int readch, char *buffer, int len);


#endif