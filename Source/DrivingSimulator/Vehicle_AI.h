// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Vehicle_AI.generated.h"

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API AVehicle_AI : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	explicit AVehicle_AI(const FObjectInitializer& ObjectInitializer);
	

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
