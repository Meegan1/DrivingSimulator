// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovement_AI.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

float FPIDController::CalcNewInput(float Error, float Position)
{
    ErrorSum = FMath::Clamp(Error + ErrorSum, ErrorMin, ErrorMax);
    float Input = Error * Proportional + ErrorSum * Integral + Derivative * (LastPosition - Position);
    LastPosition = Position;
    return Input;
}

void UVehicleMovement_AI::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);

    FVector VehicleLocation = GetOwner()->GetActorLocation();
    FVector Destination = VehicleLocation + MoveVelocity * GetWorld()->GetDeltaSeconds();
    DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Destination, FColor::Red, false, 1.f, 0, 3.f);


    FVector Distance = Destination - VehicleLocation;
    FVector InitialDistance = Destination - InitialLocation;
    FVector VehicleDirection = GetOwner()->GetActorForwardVector();

    // Throttle controller
    float ForwardFactor = FVector::DotProduct(VehicleDirection, Distance.GetSafeNormal());
    // DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 0, 30),
    //     GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() + FVector(0, 0, 30), 50.f, FColor::Blue, false, 0.01f, 0, 3.f);
    
    float Error = Distance.Size() / InitialDistance.Size() * FMath::Sign(ForwardFactor);
    float Position = 1 - Error;

    float Input = ThrottleController.CalcNewInput(Error, Position);

    const float TURN_AROUND_FACTOR = bTurningAround ? 0.3f : 0.1f;
    if (ForwardFactor < TURN_AROUND_FACTOR && (bForceMaxSpeed || Distance.Size() > 300.f))
    {
        bTurningAround = true;
        SetThrottleInput(-1.f);
    }
    else
    {
        bTurningAround = false;
        SetThrottleInput(bForceMaxSpeed ? ForwardFactor : Input);
    }

    // Steering controller
    float InitialYaw = InitialDistance.Rotation().Yaw - InitialDirection.Rotation().Yaw;
    if (InitialYaw < -180)
    {
        InitialYaw += 360;
    }
    else if (InitialYaw > 180)
    {
        InitialYaw -= 360;
    }
    float CurrentYaw = Distance.Rotation().Yaw - VehicleDirection.Rotation().Yaw;
    if (CurrentYaw < -180)
    {
        CurrentYaw += 360;
    }
    else if (CurrentYaw > 180)
    {
        CurrentYaw -= 360;
    }

    float SteeringPosition = (-CurrentYaw + 180) / 180;
    float SteeringError = 1 - SteeringPosition;

    float SteeringInput_local = SteeringController.CalcNewInput(SteeringError, SteeringPosition);
    if (bTurningAround)
    {
        SetSteeringInput(SteeringError > 0 ? -1.f : 1.f);
    }
    else
    {
        SetSteeringInput(SteeringInput_local);
    }
    SetHandbrakeInput(false);
}

void UVehicleMovement_AI::StopActiveMovement()
{
    Super::StopActiveMovement();
    InitialLocation = GetOwner()->GetActorLocation();
    InitialDirection = GetOwner()->GetActorForwardVector();
    SetHandbrakeInput(true);
    SetThrottleInput(0.f);
}
