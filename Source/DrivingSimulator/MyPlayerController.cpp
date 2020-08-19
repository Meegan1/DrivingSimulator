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
 
    DeterminePawnClass();
}
 
// Pawn Class
void AMyPlayerController::DeterminePawnClass_Implementation()
{
    if (IsLocalController()) //Only Do This Locally (NOT Client-Only, since Server wants this too!)
    {   
        
        ServerSetPawn(ADrivingSimulatorPawn::StaticClass());
        return;
    }
}

void AMyPlayerController::SetPawnTrafficController_Implementation()
{
    APawn* PawnOld = GetPawn();
    FVector Location = GetPawn()->GetActorLocation();
    FRotator Rotation = GetPawn()->GetActorRotation();

    FActorSpawnParameters ActorSpawnParameters;
    ATrafficControllerPawn* PawnNew = GetWorld()->SpawnActor<ATrafficControllerPawn>(ATrafficControllerPawn::StaticClass(), Location, Rotation, ActorSpawnParameters);
    PawnOld->Destroy();
    Possess(PawnNew);
}

bool AMyPlayerController::ServerSetPawn_Validate(TSubclassOf<APawn> InPawnClass)
{
    return true;
}
 
void AMyPlayerController::ServerSetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
    MyPawnClass = InPawnClass;

    /* Just in case we didn't get the PawnClass on the Server in time... */
    GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}
 
// Replication
void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AMyPlayerController, MyPawnClass);
}