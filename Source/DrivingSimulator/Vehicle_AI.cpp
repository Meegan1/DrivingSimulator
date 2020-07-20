// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle_AI.h"

#include "DrivingSimulatorPawn_NPC.h"

void AVehicle_AI::BeginPlay()
{
    Super::BeginPlay();
}

AVehicle_AI::AVehicle_AI(const FObjectInitializer& ObjectInitializer): AAIController(ObjectInitializer)
{
    bWantsPlayerState = true;
    PrimaryActorTick.bCanEverTick = true;
}

void AVehicle_AI::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ADrivingSimulatorPawn_NPC* Bot = Cast<ADrivingSimulatorPawn_NPC>(InPawn);

    if(Bot && Bot->BotBehavior)
    {
        if(Bot->BotBehavior->BlackboardAsset)
        {
            UseBlackboard(Bot->BotBehavior->BlackboardAsset, Blackboard);
        }
        RunBehaviorTree(Bot->BotBehavior);
    }
}
