// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointManager.h"
#include "DrawDebugHelpers.h"
#include "DrivingSimulatorPawn_NPC.h"
#include "EditorViewportClient.h"
#include "Editor.h"

void AWaypointManager::BeginPlay()
{
    Super::BeginPlay();
    ReloadGraph();

    if(HasAuthority())
        SpawnAllNPCVehicles(20);
}

void AWaypointManager::AddWaypoint()
{
    AWaypoint* Waypoint_ptr = (AWaypoint*) GetWorld()->SpawnActor(AWaypoint::StaticClass());

    // set to Waypoint folder
    Waypoint_ptr->SetFolderPath("Waypoints");

    // store in array of waypoints
    Waypoints.Add(Waypoint_ptr);
}

void AWaypointManager::ReloadGraph()
{
    WaypointsGraph.clear();
    
    std::unordered_map<AWaypoint*, vertex_t> Map; // map for retrieving vertex from waypoint
    
    /*
     * Add all waypoints to vertex list
     */
    for(auto *Waypoint : Waypoints)
    {
        Map[Waypoint] = WaypointsGraph.add_vertex(Waypoint);
    }
    
    for(auto *Waypoint : Waypoints)
    {
        vertex_t Source;
        Source = Map[Waypoint]; // get vertex for waypoint

        /*
        * Add all connections as edges to graph
        */
        for(auto *Connected_Waypoint : Waypoint->Connections)
        {
            if(Connected_Waypoint == nullptr) // skip if connected waypoint is null
                continue;

            if(Map.count(Connected_Waypoint) < 1) // skip if connected waypoint isn't loaded in manager
                continue;
            
            vertex_t Target;
            Target = Map[Connected_Waypoint];
            
            WaypointsGraph.add_edge(Source, Target); // add edge/connection between waypoints
        }
    }
}

void AWaypointManager::ReloadWaypoints()
{
    Waypoints.Empty();
    for(TObjectIterator<AWaypoint> Itr; Itr; ++Itr)
    {
        if(Itr->IsA(AWaypoint::StaticClass()))
        {
            Waypoints.Push(*Itr);
        }
    }
}

void AWaypointManager::SelectRandomWaypoint()
{
#if WITH_EDITOR
    GEditor->SelectActor(GetRandomWaypoint(), true, true, true, false);
#endif
}

AWaypoint* AWaypointManager::GetRandomWaypoint()
{
    int64 vertex_id = FMath::RandRange(0, WaypointsGraph.num_vertices() - 1);

    // boost::graph_traits<Graph>::vertex_iterator i = vertices(WaypointsGraph).first;
    // vertex_t vertex = *boost::next(i, vertex_id);

    return Waypoints[vertex_id];

    // return WaypointsGraph[vertex];
}

AWaypoint* AWaypointManager::GetNextWaypoint(AWaypoint* Waypoint)
{
    if(Waypoint->Connections.Num() <= 0)
        return nullptr;

    /*
     * Get random connected waypoint ID
     */
    int64 index = FMath::RandRange(0, Waypoint->Connections.Num() - 1);
    
    return Waypoint->Connections[index];
}

ADrivingSimulatorPawn_NPC* AWaypointManager::SpawnNPCVehicle(AWaypoint* Waypoint)
{
    FVector Location = Waypoint->GetActorLocation();

    /*
     * Calculate vector from the waypoint to the center of all connected waypoints
     */
    FVector Direction = FVector::ZeroVector;
    int ConnectionCount = 0;
    for(auto* Connection : Waypoint->Connections)
    {
        if(Connection == nullptr)
            continue;
        
        Direction += Connection->GetActorLocation();
        ConnectionCount++;
    }
    Direction = (Direction / ConnectionCount) - Waypoint->GetActorLocation();
    FRotator Rotation = Direction.Rotation();

    FActorSpawnParameters ActorSpawnParameters;
    ActorSpawnParameters.bDeferConstruction = true;
    
    ADrivingSimulatorPawn_NPC* SpawnedVehicle = GetWorld()->SpawnActor<ADrivingSimulatorPawn_NPC>(ADrivingSimulatorPawn_NPC::StaticClass(), Location, Rotation, ActorSpawnParameters);
    SpawnedVehicle->StartingWaypoint = Waypoint;
    SpawnedVehicle->WaypointManager = this;
    SpawnedVehicle->FinishSpawning(SpawnedVehicle->GetTransform());

    SpawnedVehicle->SetFolderPath("SpawnedVehicles");

    return SpawnedVehicle;
}

void AWaypointManager::SpawnAllNPCVehicles(int Quantity)
{
    int CurrentQuantity = 0;
    const int TotalWaypoints = Waypoints.Num();
    while(CurrentQuantity < Quantity)
    {
        int WaypointIndex = FMath::RandRange(0, TotalWaypoints - 1);
        SpawnNPCVehicle(Waypoints[WaypointIndex]);
        CurrentQuantity++;
    }
}

AWaypointManager::AWaypointManager() : Super()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    this->ReloadGraph();
}

void AWaypointManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR

    if(ShowWaypointSpheres)
    {
        /*
        * Iterate over every graph node
        */
        std::pair<Graph::vertex_iterator, Graph::vertex_iterator> vp;
        for (vp = boost::vertices(WaypointsGraph); vp.first != vp.second; ++vp.first)
        {
            DrawDebugSphere(GetWorld(), WaypointsGraph[*vp.first]->GetActorLocation(), 100, 30, FColor::Blue, false, -1.f);
        }
    }

    
    if(ShowGraphUI)
    {
        /*
        * Iterate over every graph edge
        */
        const FVector ArrowOffset(0, 0, 200.0f);
        std::pair<Graph::edge_iterator, Graph::edge_iterator> ep;
        for (ep = boost::edges(WaypointsGraph); ep.first != ep.second; ++ep.first)
        {
            vertex_t source = boost::source(*ep.first, WaypointsGraph);
            vertex_t target = boost::target(*ep.first, WaypointsGraph);
        
            DrawDebugDirectionalArrow(GetWorld(), WaypointsGraph[source]->GetActorLocation() + ArrowOffset,
                                      WaypointsGraph[target]->GetActorLocation() + ArrowOffset, 120.0f, FColor::Red,
                                      false, -1.f, 0, 5.0f);
        
        }
    }

#endif
}

void AWaypointManager::BeginDestroy()
{
    Super::BeginDestroy();

    WaypointsGraph.clear();
}
