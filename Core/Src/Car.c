#include "main.h"
#include "Car.h"
#include "Pwm.h"
extern float TargetA,TargetB,TargetC,TargetD;
extern float Lx, Ly;

void Car_Speed(float vx, float vy, float omega)
{
TargetA = vx - vy - omega * (Lx + Ly);
TargetB = vx + vy + omega * (Lx + Ly);
TargetC = vx + vy - omega * (Lx + Ly);
TargetD = vx - vy + omega * (Lx + Ly);
}