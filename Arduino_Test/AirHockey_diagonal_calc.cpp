#include "stdafx.h"
#include <math.h>
#define PI 3.141592654

/*Calc diagonal and angle speed 
  for all quadrants
  Pusher old position: X:300, Y:150
*/

const float mmPerRev = 2 * 60 * 0.5;  //60 teeths *2mm teeth span
const float stepsPerRev = 400;
const float HBOT_CONSTANT = mmPerRev / stepsPerRev;  //use to calc mmtosteps and stepstomm

float mmToSteps(const float mm) {
    return mm / HBOT_CONSTANT;
}

int main()
{
    float x = 300;
    float y = 150;
    
    float _x, _y;
    float diffX, diffY;

    int OCRL;
    int OCRR;

    int OCR_max;

    float diagonal;
    float angle;
    float _angle;
    float angledegrees;

    printf("X:\n");
    scanf_s("%f", &_x);
    printf("Y:\n");
    scanf_s("%f", &_y);

    printf("Speed max:\n");
    scanf_s("%d", &OCR_max);
  

    diffX = _x - x;
    diffY = _y - y;

    diagonal = sqrt((diffX*diffX) + (diffY*diffY));
    printf("Diagonal in mm: %f\n", diagonal);
    printf("Diagonal in steps: %f\n", mmToSteps(diagonal));

    angle = atan((diffX/diffY));

    angledegrees = (angle * 180) / PI;
    printf("Angle: %f\n", angledegrees);

    _angle = 90 / angledegrees;

    OCRR = OCR_max / _angle;

    printf("OCRL: %d\n", OCR_max);
    printf("OCRR: %d\n", OCRR);

}

