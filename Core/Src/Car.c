#include "main.h"
#include "Car.h"
#include "Pwm.h"
extern TargetA,TargetB,TargetC,TargetD;

void Car_Speed(vx,vy,omega)
{
    TargetA = vx - vy - omega;
    TargetB = vx + vy + omega;
    TargetC = vx + vy - omega;
    TargetD = vx - vy + omega;
}