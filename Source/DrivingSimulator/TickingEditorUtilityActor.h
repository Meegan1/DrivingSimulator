// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityActor.h"
#include "TickingEditorUtilityActor.generated.h"

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API ATickingEditorUtilityActor : public AEditorUtilityActor
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EditorTick", CallInEditor = "true"))
    void ReceiveEditorTick(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
};
