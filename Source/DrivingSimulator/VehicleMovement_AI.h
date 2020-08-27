// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PIDController.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "VehicleMovement_AI.generated.h"

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API UVehicleMovement_AI : public UWheeledVehicleMovementComponent4W
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "PID")
	FPIDController ThrottleController = FPIDController(0.5f, 0.2f, 2.0f, 0.0f, 1.2f);
	// FPIDController ThrottleController = FPIDController(0.5f, 0.2f, 2.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "PID")
	FPIDController SteeringController = FPIDController(10.0f, 0.5f, 4.0f, -0.2f, 0.2f);
	// FPIDController SteeringController = FPIDController(3.0f, 0.5f, 1.5f, 0.0f, 1.0f);

	FVector StartingLocation;
	FVector StartingDirection;
	bool bTurningAround = false;
	
public:
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual void StopActiveMovement() override;
	
	void AdjustThrottleInput(bool bForceMaxSpeed, FVector Distance, FVector StartingDistance,
	                         FVector Direction);
	void AdjustSteeringInput(FVector Distance, FVector Direction);
	
	float CalculateThrottleInput(FVector Distance, FVector StartingDistance, float ForwardFactor);
	void CalculateSteeringInput(FVector Distance, FVector Direction,
	                            float& Error, float& Input);
	
	float WrapYaw(float Yaw);
};
