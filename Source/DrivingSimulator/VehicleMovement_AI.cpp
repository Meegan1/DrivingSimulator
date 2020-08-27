// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovement_AI.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "DrivingSimulatorPawn_NPC.h"

// CSV_DEFINE_CATEGORY(PIDController, true);

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
    
    // CSV_CUSTOM_STAT(PIDController, "Throttle", Input + 1.0f, ECsvCustomStatOp::Set);

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

    // CSV_CUSTOM_STAT(PIDController, "Steering", Input + 1.0f, ECsvCustomStatOp::Set);
    
    if (bTurningAround)
        SetSteeringInput(Error > 0 ? -1.f : 1.f);
    else
        SetSteeringInput(Input);
    
    SetHandbrakeInput(false);
}

float UVehicleMovement_AI::CalculateThrottleInput(FVector Distance, FVector StartingDistance, float ForwardFactor)
{
    float Error = Distance.Size() / StartingDistance.Size() * FMath::Sign(ForwardFactor);

    return ThrottleController.CalculateInput(Error);
}

void UVehicleMovement_AI::CalculateSteeringInput(FVector Distance, FVector Direction, float& Error, float& Input)
{
    float CurrentYaw = WrapYaw(Distance.Rotation().Yaw - Direction.Rotation().Yaw);

    float SteeringPosition = (-CurrentYaw + 180) / 180;
    Error = 1 - SteeringPosition;

    Input = SteeringController.CalculateInput(Error);
}

float UVehicleMovement_AI::WrapYaw(float Yaw)
{
    if (Yaw < -180)
        return Yaw += 360;
    if (Yaw > 180)
        return Yaw -= 360;
    return Yaw;
}