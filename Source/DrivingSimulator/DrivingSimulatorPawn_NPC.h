// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrivingSimulatorPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrivingSimulatorPawn_NPC.generated.h"

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API ADrivingSimulatorPawn_NPC : public ADrivingSimulatorPawn
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ADrivingSimulatorPawn_NPC(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BotBehavior;
};
