// Fill out your copyright notice in the Description page of Project Settings.


#include "DrivingSimulatorPawn_NPC.h"


#include "VehicleMovement_AI.h"
#include "Vehicle_AI.h"
#include "UObject/ConstructorHelpers.h"

void ADrivingSimulatorPawn_NPC::BeginPlay()
{
    Super::BeginPlay();
}

ADrivingSimulatorPawn_NPC::ADrivingSimulatorPawn_NPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UVehicleMovement_AI>(AWheeledVehicle::VehicleMovementComponentName))
{   
    this->AIControllerClass = AVehicle_AI::StaticClass();

    this->BotBehavior = ConstructorHelpers::FObjectFinder<UBehaviorTree>(TEXT("/Game/VehicleAdv/Vehicle/AI/Vehicle_BT.Vehicle_BT")).Object;

    this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
