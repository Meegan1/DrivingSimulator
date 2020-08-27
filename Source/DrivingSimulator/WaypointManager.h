// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Waypoint.h"
#include "CoreMinimal.h"
#include "TickingEditorUtilityActor.h"

THIRD_PARTY_INCLUDES_START
#include "boost/graph/directed_graph.hpp"
#include "boost/graph/adjacency_matrix.hpp"
#include "boost/graph/compressed_sparse_row_graph.hpp"
THIRD_PARTY_INCLUDES_END

#include "WaypointManager.generated.h"


// directed_graph is a subclass of adjacency_list which gives you object oriented access to functions
// like add_vertex and add_edge, which makes the code easier to understand. However, it hard codes many
// of the template parameters, so it is much less flexible.
typedef boost::directed_graph<AWaypoint*> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;


class ADrivingSimulatorPawn_NPC;

/**
 * 
 */
UCLASS()
class DRIVINGSIMULATOR_API AWaypointManager : public ATickingEditorUtilityActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(CallInEditor, Category="Waypoint Graph")
	void AddWaypoint();
	UFUNCTION(CallInEditor, Category="Waypoint Graph")
	void ReloadGraph();
	UFUNCTION(CallInEditor, Category="Waypoint Graph")
	void ReloadWaypoints();
	UFUNCTION(CallInEditor, Category="Waypoint Graph")
	void SelectRandomWaypoint();

	UFUNCTION(BlueprintCallable)
	AWaypoint* GetRandomWaypoint();
	
	UFUNCTION(BlueprintCallable)
	AWaypoint* GetNextWaypoint(AWaypoint* Waypoint);

	UFUNCTION(BlueprintCallable)
	ADrivingSimulatorPawn_NPC* SpawnNPCVehicle(AWaypoint* Waypoint);

	void SpawnAllNPCVehicles(int Quantity);

	Graph WaypointsGraph;
public:
	AWaypointManager();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

	UPROPERTY(EditAnywhere)
	TArray<AWaypoint*> Waypoints;
	
	UPROPERTY(EditAnywhere, Category="Waypoint Graph")
	bool ShowGraphUI = true;
	UPROPERTY(EditAnywhere, Category="Waypoint Graph")
	bool ShowWaypointSpheres = false;
};
