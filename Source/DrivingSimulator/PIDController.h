#pragma once

#include "CoreMinimal.h"
#include "PIDController.generated.h"

USTRUCT()
struct FPIDController
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "PID")
    float Proportional;

    UPROPERTY(EditAnywhere, Category = "PID")
    float Integral;

    UPROPERTY(EditAnywhere, Category = "PID")
    float Derivative;

    UPROPERTY(EditAnywhere, Category = "PID")
    float ErrorMin;

    UPROPERTY(EditAnywhere, Category = "PID")
    float ErrorMax;

    float ErrorSum;
    float LastError;

    FPIDController();

    FPIDController(float P, float I, float D, float ErrorMin, float ErrorMax);

    float CalculateInput(float Error);
};