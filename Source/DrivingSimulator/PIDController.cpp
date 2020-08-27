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

float FPIDController::CalculateInput(float Error)
{
    ErrorSum = FMath::Clamp(Error + ErrorSum, ErrorMin, ErrorMax);
    float Input = (Error * Proportional) + (ErrorSum * Integral) + (Derivative * (Error - LastError));
    LastError = Error;
    return Input;
}
