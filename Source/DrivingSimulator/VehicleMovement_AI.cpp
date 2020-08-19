// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovement_AI.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "DrivingSimulatorPawn_NPC.h"

void UVehicleMovement_AI::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);

    /*
     * Get vehicle location and destination
     */
    FVector VehicleLocation = GetOwner()->GetActorLocation();
    FVector Destination = VehicleLocation + MoveVelocity * GetWorld()->GetDeltaSeconds();

#if WITH_EDITOR
    /*
    * Debug line from vehicle to destination
    */
    ADrivingSimulatorPawn_NPC* Owner = Cast<ADrivingSimulatorPawn_NPC>(GetOwner());

    // if ShowGraphUI is enabled in waypoint manager
    if(Owner != nullptr && Owner->WaypointManager->ShowGraphUI)
        DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Destination, FColor::Red, false, 1.f, 0, 3.f);
#endif

    FVector Distance = Destination - VehicleLocation; // distance between vehicle and destination
    FVector StartingDistance = Destination - StartingLocation; // distance between destination and starting location
    FVector VehicleDirection = GetOwner()->GetActorForwardVector();

    
    /*
    * Throttle Controller
    */
    AdjustThrottleInput(bForceMaxSpeed, Distance, StartingDistance, VehicleDirection);

    /*
    * Steering controller
    */
    AdjustSteeringInput(Distance, VehicleDirection);
}

void UVehicleMovement_AI::StopActiveMovement()
{
    Super::StopActiveMovement();
    StartingLocation = GetOwner()->GetActorLocation();
    StartingDirection = GetOwner()->GetActorForwardVector();
    SetHandbrakeInput(true);
    SetThrottleInput(0.f);
}


void UVehicleMovement_AI::AdjustThrottleInput(bool bForceMaxSpeed, FVector Distance, FVector StartingDistance, FVector Direction)
{
    float ForwardFactor = FVector::DotProduct(Direction, Distance.GetSafeNormal());
    float Input = CalculateThrottleInput(Distance, StartingDistance, ForwardFactor);

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
}

void UVehicleMovement_AI::AdjustSteeringInput(FVector Distance, FVector Direction)
{
    float Error;
    float Input;
    CalculateSteeringInput(Distance, Direction, Error, Input);
    
    if (bTurningAround)
        SetSteeringInput(Error > 0 ? -1.f : 1.f);
    else
        SetSteeringInput(Input);
    
    SetHandbrakeInput(false);
}

float UVehicleMovement_AI::CalculateThrottleInput(FVector Distance, FVector StartingDistance, float ForwardFactor)
{
    float Error = Distance.Size() / StartingDistance.Size() * FMath::Sign(ForwardFactor);
    float Position = 1 - Error;

    return ThrottleController.CalcNewInput(Error, Position);
}

void UVehicleMovement_AI::CalculateSteeringInput(FVector Distance, FVector Direction, float& Error, float& Input)
{
    float CurrentYaw = ClampYaw(Distance.Rotation().Yaw - Direction.Rotation().Yaw);

    float SteeringPosition = (-CurrentYaw + 180) / 180;
    Error = 1 - SteeringPosition;

    Input = SteeringController.CalcNewInput(Error, SteeringPosition);
}

float UVehicleMovement_AI::ClampYaw(float Yaw)
{
    if (Yaw < -180)
        return Yaw += 360;
    if (Yaw > 180)
        return Yaw -= 360;
    return Yaw;
}