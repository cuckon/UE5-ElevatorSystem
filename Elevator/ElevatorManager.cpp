// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorManager.h"
#include "GateBase.h"
#include "ElevatorBase.h"
#include "EaseMoveComponent.h"
#include <Components/SceneComponent.h>

PRAGMA_DISABLE_OPTIMIZATION


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
	bool PickTheUp = Elevator->GetState() == ElevatorState::kUp || Elevator->GetState() == ElevatorState::kStopped;
	auto& GateIndices = PickTheUp ? PendingUpGates : PendingDownGates;

	for (auto GateIdx : GateIndices) {
		if (CanPickGateOnThisRide(Elevator, GateIdx)) {
			Elevator->MoveToGate(GateIdx);
			break;
		}
	}
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
		Elevator->ArrivalUpDelegates.AddDynamic(this, &AElevatorManager::OnAnyArrivalUp);
		Elevator->ArrivalDownDelegates.AddDynamic(this, &AElevatorManager::OnAnyArrivalDown);
	}
	for (auto Gate : Gates) {
		Gate->SetManager(this);
		Gate->PendingDownDelegates.AddDynamic(this, &AElevatorManager::OnAnyPendingDown);
		Gate->PendingUpDelegates.AddDynamic(this, &AElevatorManager::OnAnyPendingUp);
	}

}


void AElevatorManager::OnAnyPending(bool IsUp, int GateIdx) {
	auto& GateIndices = IsUp ? PendingUpGates : PendingDownGates;
	GateIndices.Add(GateIdx);
	GateIndices.Sort();
	if (!IsUp)
		Algo::Reverse(GateIndices);

	for (auto Elevator : Elevators)
		Schedule(Elevator);
}

bool AElevatorManager::CanPickGateOnThisRide(AElevatorBase* Elevator, int GateIdx)
{
	ElevatorState State = Elevator->GetState();
	if (State == ElevatorState::kStopped)
		return true;

	float GateHeight = Gates[GateIdx]->GetActorLocation().Z;
	float CurrentHeight = Elevator->GetActorLocation().Z;
	
	return State == ElevatorState::kUp && GateHeight > CurrentHeight || State == ElevatorState::kDown && GateHeight < CurrentHeight;
}


void AElevatorManager::OnAnyPendingUp_Implementation(int GateIdx)
{
	AElevatorManager::OnAnyPending(true, GateIdx);
}

void AElevatorManager::OnAnyPendingDown_Implementation(int GateIdx)
{
	AElevatorManager::OnAnyPending(false, GateIdx);
}

void AElevatorManager::OnAnyArrivalUp_Implementation(int GateIdx, int ElevatorIdx)
{
	PendingUpGates.RemoveAt(0);
}

void AElevatorManager::OnAnyArrivalDown_Implementation(int GateIdx, int ElevatorIdx)
{
	PendingDownGates.RemoveAt(0);
}


// Called every frame
void AElevatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

