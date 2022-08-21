// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorManager.h"
#include "GateBase.h"
#include "ElevatorBase.h"
#include "EaseMoveComponent.h"
#include <Components/SceneComponent.h>


// Sets default values
AElevatorManager::AElevatorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));
	SceneComponent->SetupAttachment(RootComponent);

}

void AElevatorManager::AddAllGates()
{
	UE_LOG(LogTemp, Log, TEXT("AddAllGates"));
}

void AElevatorManager::AddAllElevators()
{
	UE_LOG(LogTemp, Log, TEXT("AddAllElevators"));
}

void AElevatorManager::Schedule(AElevatorBase* Elevator)
{
}

// Called when the game starts or when spawned
void AElevatorManager::BeginPlay()
{
	Super::BeginPlay();

	Algo::Sort(Gates, [](const AGateBase* A, const AGateBase* B) {
		return A->GetActorLocation().Z < B->GetActorLocation().Z;
	});

	for (auto Elevator : Elevators) {
		Elevator->SetManager(this);
		Elevator->ArrivalDelegates.AddUniqueDynamic(this, &AElevatorManager::OnAnyArrival);
	}
	for (auto Gate : Gates) {
		Gate->SetManager(this);
		Gate->PendingDownDelegates.AddUniqueDynamic(this, &AElevatorManager::OnAnyPending);
	}

	AElevatorBase* Elevator = Elevators[0];
	
	// Todo : Replace this
	Elevator->Roadmap.Push(3);
	Elevator->Roadmap.Push(2);
	Elevator->Roadmap.Push(3);
	Elevator->Roadmap.Push(0);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, Elevator, &AElevatorBase::StartNextMove, 1.5, false);
}


void AElevatorManager::OnAnyPending_Implementation(int GateIdx)
{

}

void AElevatorManager::OnAnyArrival_Implementation(int GateIdx, int ElevatorIdx)
{
	UE_LOG(
		LogTemp, Log, TEXT("Gate: %d Elevator: %d"),
		GateIdx, ElevatorIdx
	);
}


// Called every frame
void AElevatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

