// Copyright Epic Games, Inc. All Rights Reserved.

#include "DrivingSimulatorGameMode.h"
#include "DrivingSimulatorPawn.h"
#include "DrivingSimulatorHud.h"
#include "MyPlayerController.h"
#include "TrafficControllerPawn.h"

ADrivingSimulatorGameMode::ADrivingSimulatorGameMode()
{
	HUDClass = ADrivingSimulatorHud::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass = ATrafficControllerPawn::StaticClass();
}
