// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointManager.h"
#include "DrawDebugHelpers.h"
#include "EditorViewportClient.h"
#include "Editor.h"



void AWaypointManager::AddWaypoint()
{
    AWaypoint* Waypoint_ptr = (AWaypoint*) GetWorld()->SpawnActor(AWaypoint::StaticClass());

    // set to Waypoint folder
    Waypoint_ptr->SetFolderPath("Waypoints");

    // store in array of waypoints
    Waypoints.Add(Waypoint_ptr);

#if UE_EDITOR
    GEditor->SelectActor(Waypoint_ptr, true, true, true, false);
    FEditorViewportClient* client = (FEditorViewportClient*) GEditor->GetActiveViewport()->GetClient();

    FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
    RV_TraceParams.bTraceComplex = true;
    RV_TraceParams.bReturnPhysicalMaterial = false;
 
    //Re-initialize hit info
    FHitResult RV_Hit(ForceInit);

    FVector CameraLocation = client->GetViewLocation();
    FVector CameraEnd = CameraLocation + FRotationMatrix(client->GetViewRotation()).GetScaledAxis(EAxis::X).Normalize() * 1500;
     
    //call GetWorld() from within an actor extending class
    GetWorld()->LineTraceSingleByChannel(
        RV_Hit,        //result
        CameraLocation,    //start
        CameraEnd, //end
        ECC_WorldStatic, //collision channel
        RV_TraceParams
    );

    Waypoint_ptr->SetActorLocation(RV_Hit.Location);
 
#endif
}

void AWaypointManager::ReloadGraph()
{
    WaypointsGraph.clear();


    std::unordered_map<AWaypoint*, vertex_t> Map;
    
    for(auto *Waypoint : Waypoints)
    {
        Map[Waypoint] = WaypointsGraph.add_vertex(Waypoint);
    }

    for(auto *Waypoint : Waypoints)
    {
        vertex_t Source;
        Source = Map[Waypoint];
        for(auto *Connected_Waypoint : Waypoint->Connections)
        {
            if(Connected_Waypoint == nullptr)
                continue;
            
            vertex_t Target;
            Target = Map[Connected_Waypoint];
            
            WaypointsGraph.add_edge(Source, Target);
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

    boost::graph_traits<Graph>::vertex_iterator i = vertices(WaypointsGraph).first;
    vertex_t vertex = *boost::next(i, vertex_id);

    return WaypointsGraph[vertex];
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

    /*
     * Iterate over every graph node
     */
    std::pair<Graph::vertex_iterator, Graph::vertex_iterator> vp;
    for (vp = boost::vertices(WaypointsGraph); vp.first != vp.second; ++vp.first)
    {
        DrawDebugSphere(GetWorld(), WaypointsGraph[*vp.first]->GetActorLocation(), 100, 30, FColor::Blue);
    }

    
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

void AWaypointManager::BeginDestroy()
{
    Super::BeginDestroy();

    WaypointsGraph.clear();
}
