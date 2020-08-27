// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"



#include "DrawDebugHelpers.h"
#include "DrivingSimulatorPawn.h"
#include "TrafficControllerPawn.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    /* Make sure the PawnClass is Replicated */
    bReplicates = true;
}
 
void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AMyPlayerController::SetPawnTrafficController_Implementation()
{
    APawn* PawnOld = GetPawn();
    FVector Location = PawnOld->GetActorLocation();
    FRotator Rotation = PawnOld->GetActorRotation();

    /*
     * Spawn new actor
     */
    FActorSpawnParameters ActorSpawnParameters;
    ATrafficControllerPawn* PawnNew = GetWorld()->SpawnActor<ATrafficControllerPawn>(ATrafficControllerPawn::StaticClass(), Location, Rotation, ActorSpawnParameters);
    PawnOld->Destroy(); // destroy old actor
    Possess(PawnNew); // possess new actor
}

void AMyPlayerController::SetPawnVehicle_Implementation()
{
    APawn* PawnOld = GetPawn();
    FVector Location = PawnOld->GetActorLocation();
    FRotator Rotation = PawnOld->GetActorRotation();
    
    /*
    * Spawn new actor
    */
    FActorSpawnParameters ActorSpawnParameters;
    ADrivingSimulatorPawn* PawnNew = GetWorld()->SpawnActor<ADrivingSimulatorPawn>(ADrivingSimulatorPawn::StaticClass(), Location, Rotation, ActorSpawnParameters);
    PawnOld->Destroy(); // destroy old actor
    Possess(PawnNew); // possess new actor
}