// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficControllerPawn.h"

// Sets default values
ATrafficControllerPawn::ATrafficControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATrafficControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrafficControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

