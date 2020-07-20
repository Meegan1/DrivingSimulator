// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Waypoint.h"
#include "CoreMinimal.h"
#include "TickingEditorUtilityActor.h"

THIRD_PARTY_INCLUDES_START
#include "boost/graph/directed_graph.hpp"
THIRD_PARTY_INCLUDES_END

#include "WaypointManager.generated.h"


USTRUCT()
struct FGraphVertexInfo
{
	GENERATED_BODY()
	
	TSharedPtr<AWaypoint> Waypoint;
};
typedef boost::directed_graph<FGraphVertexInfo> Graph;

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API AWaypointManager : public ATickingEditorUtilityActor
{
	GENERATED_BODY()

private:
	UFUNCTION(CallInEditor)
	void AddWaypoint();

	Graph Waypoints;
public:
	AWaypointManager();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

};
