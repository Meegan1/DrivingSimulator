// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointManager.h"

#include "DrawDebugHelpers.h"
#include "boost/graph/compressed_sparse_row_graph.hpp"


void AWaypointManager::AddWaypoint()
{
    // directed_graph is a subclass of adjacency_list which gives you object oriented access to functions
    // like add_vertex and add_edge, which makes the code easier to understand. However, it hard codes many
    // of the template parameters, so it is much less flexible.

    FGraphVertexInfo Waypoint;

    AWaypoint* Waypoint_ptr = (AWaypoint*) GetWorld()->SpawnActor(AWaypoint::StaticClass());
    Waypoint.Waypoint = TSharedPtr<AWaypoint>(Waypoint_ptr);
    boost::graph_traits<Graph>::vertex_descriptor v0 = Waypoints.add_vertex(Waypoint);
}

AWaypointManager::AWaypointManager() : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AWaypointManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    std::pair<Graph::vertex_iterator, Graph::vertex_iterator> vp;
    for (vp = boost::vertices(Waypoints); vp.first != vp.second; ++vp.first)
    {
        DrawDebugSphere(GetWorld(), Waypoints[*vp.first].Waypoint->GetActorLocation(), 100, 30, FColor::Red);
        GEngine->AddOnScreenDebugMessage(0, 4.0f, FColor::White, TEXT("Text"), true);
    }
    GEngine->AddOnScreenDebugMessage(0, 4.0f, FColor::White, TEXT("Text 2"), true);
}

void AWaypointManager::BeginDestroy()
{
    Super::BeginDestroy();

    Waypoints.clear();
}
