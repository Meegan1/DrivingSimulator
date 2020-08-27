// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	/* Constructor */
	AMyPlayerController(const FObjectInitializer& ObjectInitializer);
 
	protected:
	UFUNCTION(Reliable, Server, BlueprintCallable)
    void SetPawnTrafficController();
	virtual void SetPawnTrafficController_Implementation();
	UFUNCTION(Reliable, Server, BlueprintCallable)
    void SetPawnVehicle();
	virtual void SetPawnVehicle_Implementation();

	virtual void BeginPlay() override;
};
