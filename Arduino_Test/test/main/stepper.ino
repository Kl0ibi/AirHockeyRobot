/*
   stepper.c

   Created: 22.07.2021 15:11:31
    Author: Florian
*/
#include "constants.h"

//init Steppers
void init_steppers()
{
  DDR_PUL_L |= (1 << PIN_PUL_L);
  DDR_PUL_R |= (1 << PIN_PUL_R);
  DDR_DIR_L |= (1 << PIN_DIR_L);
  DDR_DIR_R |= (1 << PIN_DIR_R);
}

//Toggle Directions
void changeDirection_L()
{
  PORT_DIR_L ^= (1 << PIN_DIR_L);
  delayMicroseconds(5);

}

void changeDirection_R()
{
  PORT_DIR_R ^= (1 << PIN_DIR_R);
  delayMicroseconds(5);
}

void Steppermoveleft()
{
  if (!error)
  {
    PORT_DIR_L &= ~(1 << PIN_DIR_L); //direction 0 =stepper left = pusher right
    PORT_DIR_R &= ~(1 << PIN_DIR_R);
    TCCR3B |= (1 << CS31);
    TIMSK3 |= (1 << OCIE3A);
  }
}

void Steppermoveright()
{
  if (!error)
  {
    PORT_DIR_L |= (1 << PIN_DIR_L); //direction 1 =stepper right = pusher left
    PORT_DIR_R |= (1 << PIN_DIR_R);
    TCCR3B |= (1 << CS31);
    TIMSK3 |= (1 << OCIE3A);
  }
}

void Steppermoveforward()
{
  if (!error)
  {
    PORT_DIR_L |= (1 << PIN_DIR_L);
    PORT_DIR_R &= ~(1 << PIN_DIR_R);
    TCCR3B |= (1 << CS31);
    TIMSK3 |= (1 << OCIE3A);
  }
}

void Steppermovebackward()
{
  if (!error)
  {
    PORT_DIR_L &= ~(1 << PIN_DIR_L);
    PORT_DIR_R |= (1 << PIN_DIR_R);
    TCCR3B |= (1 << CS31);
    TIMSK3 |= (1 << OCIE3A);
  }
}

void Stepperstop()
{
  TCCR3B &= ~(1 << CS31);
  TIMSK3 &= ~(1 << OCIE3A);
  cntsteps = 0;
}

void setMaximalSpeed()
{
  OCR3A = 500;
}

void setDesiredSpeed(int value)
{
  if (value >= 400 && value <= 65535)
  {
    OCR3A = value;
  }
}

void setAccel(int accel)
{
  if(accel==0)
  {
   OCR3A = 700;
  }else if (accel==1)
  {
    OCR3A = 500;
  }else if(accel==2)
  {
    OCR3A = 400;
  }else if (accel==3)
  {
    OCR3A = 200;
  }
}

void setDefaultParams()
{
  OCR3A = 700;
}

void SteppermovetoXY(float _x, float _y)
{
  float x_mm;
  float x_steps;
  float y_mm;
  float y_steps;
  int t = 0;
  int i = 0;
  if (!error)
  {
    if (_x <= BARRIER_X_MAX && _x >= BARRIER_X_MIN-10 && _y <= BARRIER_Y_MAX && _y >= BARRIER_Y_MIN)
    {
      x_mm = x - _x;
      if (x_mm <= 0)
      {
        x_steps = mmToSteps(-x_mm);
        Steppermoveright();
        while (cntsteps <= x_steps)
        { if (-x_mm >= 60) {
            if (cntsteps >= (x_steps / 80) && i <= 3)
            {
              setAccel(i);
              i++;
              if(i==3)
              {
              t=3;
              }
            }
            if (cntsteps >= ((x_steps) - (x_steps / 20)) && i >= 3 && t>0)
            {
              setAccel(t);
              t--;
              
            }
          }
          checkDriverError();
        }
        Stepperstop();
        i = 0;
        
      } else
      {
        x_steps = mmToSteps(x_mm);

        Steppermoveleft();
        while (cntsteps <= x_steps)
        {
          if (x_mm >= 60) {
            if (cntsteps >= (x_steps / 80) && i <= 3)
            {
              setAccel(i);
              i++;
              if(i==3)
              {
              t=3;
              }
            }
            if (cntsteps >= ((x_steps) - (x_steps / 20)) && i >= 3 && t>0)
            {
              setAccel(t);
              t--;
            }
          }
          checkDriverError();
        }
        Stepperstop();
        i = 0;
        t=0;
      }

      y_mm = y - _y;

      if (y_mm <= 0)
      {
        y_steps = mmToSteps(-y_mm);

        Steppermoveforward();
        while (cntsteps <= y_steps)
        {
          if (-y_mm >= 40) {
            if (cntsteps >= (y_steps / 80) && i <= 3)
            {
              setAccel(i);
              i++;
              if(i==3)
              {
              t=3;
              }
            }
            if (cntsteps >= ((y_steps) - (y_steps / 40)) && i >= 3 && t>0)
            {
              setAccel(t);
              t--;
            }
          }
          checkDriverError();
        }
        Stepperstop();
        i = 0;
        t=0;
      } else
      {
        y_steps = mmToSteps(y_mm);
        Steppermovebackward();
        while (cntsteps <= y_steps)
        {
          if (-y_mm >= 40) {
            if (cntsteps >= (y_steps / 40) && i <= 3)
            {
              setAccel(i);
              i++;
              if(i==3)
              {
              t=3;
              }
            }
            if (cntsteps >= ((y_steps) - (y_steps / 40)) && i >= 3 && t>0)
            {
              setAccel(t);
              t--;
            }
          }
          checkDriverError();
        }
        Stepperstop();
        i = 0;
      }
      x = _x;
      y = _y;
    } else
    {
      Serial.println("Coordinates not in the permitted area or is already there");
    }
  }
}
