// Fill out your copyright notice in the Description page of Project Settings.


#include "TickingEditorUtilityActor.h"

void ATickingEditorUtilityActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    #if WITH_EDITOR
        ReceiveEditorTick(DeltaTime);
    #endif
}

bool ATickingEditorUtilityActor::ShouldTickIfViewportsOnly() const
{
    return true;
}