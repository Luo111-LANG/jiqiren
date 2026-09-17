#include "main.h"
#include "Car.h"
#include "Pwm.h"

void Car_Stop(void)
{
  SetPWMA(0);
  SetPWMB(0);
  SetPWMC(0);
  SetPWMD(0);
}

void Car_Forward(void)
{
  SetPWMA(30);
  SetPWMB(30);
  SetPWMC(30);
  SetPWMD(30);
}

void Car_Backward(void)
{
  SetPWMA(-30);
  SetPWMB(-30);
  SetPWMC(-30);
  SetPWMD(-30);
}

void Car_Left(void)
{
  SetPWMA(-30);
  SetPWMB(30);
  SetPWMC(30);
  SetPWMD(-30);
}

void Car_Right(void)
{
  SetPWMA(30);
  SetPWMB(-30);
  SetPWMC(-30);
  SetPWMD(30);
}

void Car_LeftTurn(void)
{
  SetPWMA(-30);
  SetPWMB(30);
  SetPWMC(-30);
  SetPWMD(30);
}

void Car_RightTurn(void)
{
  SetPWMA(30);
  SetPWMB(-30);
  SetPWMC(30);
  SetPWMD(-30);
}