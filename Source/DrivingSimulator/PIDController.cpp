#include "PIDController.h"


FPIDController::FPIDController()
{
}

FPIDController::FPIDController(float P, float I, float D, float ErrorMin, float ErrorMax)
{
    Proportional = P;
    Integral = I;
    Derivative = D;
    this->ErrorMin = ErrorMin;
    this->ErrorMax = ErrorMax;
}

float FPIDController::CalcNewInput(float Error, float Position)
{
    ErrorSum = FMath::Clamp(Error + ErrorSum, ErrorMin, ErrorMax);
    float Input = Error * Proportional + ErrorSum * Integral + Derivative * (LastPosition - Position);
    LastPosition = Position;
    return Input;
}
