// Copyright Epic Games, Inc. All Rights Reserved.

#include "DrivingSimulatorGameMode.h"
#include "DrivingSimulatorPawn.h"
#include "DrivingSimulatorHud.h"
#include "MyPlayerController.h"

ADrivingSimulatorGameMode::ADrivingSimulatorGameMode()
{
	HUDClass = ADrivingSimulatorHud::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass = ADrivingSimulatorPawn::StaticClass();
}

UClass* ADrivingSimulatorGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(InController);

	if(PlayerController)
		return PlayerController->GetPlayerPawnClass();

	return DefaultPawnClass;
}
