// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorManager.h"
#include "GateBase.h"
#include "ElevatorBase.h"
#include "EaseMoveComponent.h"
#include <Components/SceneComponent.h>

PRAGMA_DISABLE_OPTIMIZATION

const int kNoElevatorAvailableIdx = -1;


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

void AElevatorManager::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	Algo::Sort(Gates, [](const AGateBase* A, const AGateBase* B) {
		return A->GetActorLocation().Z < B->GetActorLocation().Z;
		});

	for (auto Elevator : Elevators) {
		Elevator->SetManager(this);
		Elevator->ArrivalUpDelegates.AddDynamic(this, &AElevatorManager::OnArrivalUp);
		Elevator->ArrivalDownDelegates.AddDynamic(this, &AElevatorManager::OnArrivalDown);
	}
	for (auto Gate : Gates) {
		Gate->SetManager(this);
		Gate->PendingDownDelegates.AddDynamic(this, &AElevatorManager::OnPendingDown);
		Gate->PendingUpDelegates.AddDynamic(this, &AElevatorManager::OnPendingUp);
	}
	UE_LOG(LogInit, Log, TEXT("PreInitializeComponents"));
}

// Called when the game starts or when spawned
void AElevatorManager::BeginPlay()
{
	Super::BeginPlay();
}


void AElevatorManager::OnAnyPending(bool IsUp, int GateIdx) {
	auto& GateIndices = IsUp ? PendingGatesUp : PendingGatesDown;
	GateIndices.Add(GateIdx);
	GateIndices.Sort();
	if (!IsUp)
		Algo::Reverse(GateIndices);

	int ElevatorIdx = BestElevatorForPendingGate(GateIdx, IsUp);
	if (ElevatorIdx == kNoElevatorAvailableIdx)
		return;

	Elevators[ElevatorIdx]->MoveToGate(GateIdx, IsUp ? ElevatorState::kUp : ElevatorState::kDown);
}

int AElevatorManager::BestElevatorForPendingGate(int GateIdx, bool ForUp) const
{
	// TODO: Choose the nearest available elevator.
	//bool CanPickTheUp;
	for (int i = 0; i < Elevators.Num(); i++) {
		auto Elevator = Elevators[i];
		if (Elevator->GetState() == ElevatorState::kStopped)
			return i;
		/*CanPickTheUp = Elevator->GetState() == ElevatorState::kUp;
		if (CanPickTheUp == ForUp)
			return i;*/
	}

	return kNoElevatorAvailableIdx;
}

bool AElevatorManager::CanPickGateOnThisRide(AElevatorBase* Elevator, int GateIdx) const
{
	ElevatorState State = Elevator->GetState();
	if (State == ElevatorState::kStopped)
		return true;

	float GateHeight = Gates[GateIdx]->GetActorLocation().Z;
	float CurrentHeight = Elevator->GetActorLocation().Z;
	
	return State == ElevatorState::kUp && GateHeight > CurrentHeight || State == ElevatorState::kDown && GateHeight < CurrentHeight;
}

void AElevatorManager::GetUntakenPendingGates(bool Up, TArray<int>& out) const
{
	TArray<int> Taken;
	GetTakenPendingGates(Up, Taken);

	out.Empty();
	for (auto GateIdx : (Up ? PendingGatesUp : PendingGatesDown)) {
		if (!Taken.Contains(GateIdx))
			out.Add(GateIdx);
	}
}

void AElevatorManager::GetTakenPendingGates(bool Up, TArray<int>& out) const
{
	out.Empty();
	for (auto Elevator : Elevators) 
		if (Elevator->GetState() != ElevatorState::kStopped)
			out.Add(Elevator->TargetGateIdx);
}


void AElevatorManager::OnPendingUp_Implementation(int GateIdx)
{
	AElevatorManager::OnAnyPending(true, GateIdx);
}

void AElevatorManager::OnPendingDown_Implementation(int GateIdx)
{
	AElevatorManager::OnAnyPending(false, GateIdx);
}

void AElevatorManager::OnArrivalUp_Implementation(int GateIdx, int ElevatorIdx)
{
	OnAnyArrival(GateIdx, ElevatorIdx);
}

void AElevatorManager::OnArrivalDown_Implementation(int GateIdx, int ElevatorIdx)
{
	OnAnyArrival(GateIdx, ElevatorIdx);
}

void AElevatorManager::OnAnyArrival(int GateIdx, int ElevatorIdx)
{
	bool MovingUp = Elevators[ElevatorIdx]->GetReasonOfMoving() == ElevatorState::kUp;
	auto& PendingGates = (MovingUp ? PendingGatesUp : PendingGatesDown);
	PendingGates.RemoveAt(0);
	((*Gates[GateIdx]).*(MovingUp ? &AGateBase::StartedUp : &AGateBase::StartedDown))();

	// Ready to fire to the next gate. Pick the gate
	TArray<int> UntakenGates;
	GetUntakenPendingGates(MovingUp, UntakenGates);

	if (UntakenGates.IsEmpty()) {
		MovingUp = !MovingUp;
		GetUntakenPendingGates(MovingUp, UntakenGates);
	}
	if (!UntakenGates.IsEmpty())
		Elevators[ElevatorIdx]->MoveToGate(UntakenGates[0], MovingUp ? ElevatorState::kUp : ElevatorState::kDown);
}


// Called every frame
void AElevatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

