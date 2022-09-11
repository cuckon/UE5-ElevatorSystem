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
		Elevator->Manager = this;
		Elevator->ArrivalUpDelegates.AddDynamic(this, &AElevatorManager::OnArrivalUp);
		Elevator->ArrivalDownDelegates.AddDynamic(this, &AElevatorManager::OnArrivalDown);
		Elevator->ReadyToGoDelegates.AddDynamic(this, &AElevatorManager::OnElevatorReadyToGo);
	}
	for (auto Gate : Gates) {
		Gate->Manager = this;
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
	int ElevatorIdx = BestElevatorForPendingGate(GateIdx, IsUp);
	if (ElevatorIdx == kNoElevatorAvailableIdx)
		return;

	Elevators[ElevatorIdx]->MoveToGate(GateIdx, IsUp ? ElevatorState::kUp : ElevatorState::kDown);
}

int AElevatorManager::NearestElevatorToGate(int GateIdx, TArray<int>& IdleElevatorIndices) const
{
	float GateHeight = Gates[GateIdx]->GetActorLocation().Z;
	Algo::Sort(
		IdleElevatorIndices,
		[&](int iA, int iB) {
			float HeightA = Elevators[iA]->GetActorLocation().Z;
			float HeightB = Elevators[iB]->GetActorLocation().Z;
			return FMath::Abs(HeightA - GateHeight) < FMath::Abs(HeightB - GateHeight);
		}
	);
	return IdleElevatorIndices[0];
}

int AElevatorManager::BestElevatorForPendingGate(int GateIdx, bool ForUp) const
{
	TArray<int> ElevatorIndices;

	// Find nearest stopped elevator first.
	for (int i = 0; i < Elevators.Num(); i++)
		if (Elevators[i]->State == ElevatorState::kStandby)
			ElevatorIndices.Add(i);

	if (!ElevatorIndices.IsEmpty())
		return NearestElevatorToGate(GateIdx, ElevatorIndices);

	// If that fails, finds nearest elevators that can pick up the gate.
	for (int i = 0; i < Elevators.Num(); i++) {
		auto Elevator = Elevators[i];

		// Check if can pick it up on its way.
		if (ForUp != (Elevator->State == ElevatorState::kUp))
			continue;

		float IntermediateHeight = Elevator->EaseMove->GetIntermediatePosition().Z;
		float GateHeight = Gates[GateIdx]->GetActorLocation().Z;
		if (ForUp && IntermediateHeight < GateHeight || !ForUp && IntermediateHeight > GateHeight)
			ElevatorIndices.Add(i);
	}
	if (!ElevatorIndices.IsEmpty())
		return NearestElevatorToGate(GateIdx, ElevatorIndices);

	return kNoElevatorAvailableIdx;
}

// Find next index to go using LOOK algrithm. Find A first then B then A.
// Returns a std::tuple of the found idx and direction to consume.
// Returns -1 if A and B are both empty.
// Reference: https://www.youtube.com/watch?v=cxwUdPpva2Y
std::tuple<int, bool> LookNext(const TArray<int>& A, const TArray<int>& B, int SplitIdx, bool IsUp) {
	if (A.IsEmpty() && B.IsEmpty())
		return std::make_tuple(-1, false);

	for (int i = 0; i < A.Num(); i++) 
		if (IsUp && (A[i] > SplitIdx) || !IsUp && (A[i] < SplitIdx))
			return std::make_tuple(A[i], IsUp);

	if (!B.IsEmpty())
		return std::make_tuple(B[0], !IsUp);

	return std::make_tuple(A[0], IsUp);
}

void AElevatorManager::Schedule(int ElevatorIdx, bool PreferUp, int SplitGateIdx)
{
	TArray<int> UntakenGatesUp, UntakenGatesDown;
	GetUntakenPendingGates(true, UntakenGatesUp);
	GetUntakenPendingGates(false, UntakenGatesDown);

	if (UntakenGatesUp.IsEmpty() && UntakenGatesDown.IsEmpty())
		return;
	
	auto [NextIdx, GoUp] = PreferUp ? LookNext(UntakenGatesUp, UntakenGatesDown, SplitGateIdx, true)
		: LookNext(UntakenGatesDown, UntakenGatesUp, SplitGateIdx, false);

	Elevators[ElevatorIdx]->MoveToGate(NextIdx, GoUp ? ElevatorState::kUp : ElevatorState::kDown);
}

// Returns the unoccupied gates indices. If Up is false, the order is reversed.
void AElevatorManager::GetUntakenPendingGates(bool Up, TArray<int>& out) const
{
	TArray<int> Taken;
	GetTakenPendingGates(Up, Taken);
	out.Empty();

	for (int i = 0; i < Gates.Num(); i++) {
		if (!(Up ? Gates[i]->IsPendingUp : Gates[i]->IsPendingDown))
			continue;
		if (Taken.Contains(i))
			continue;
		out.Add(i);
	}
	if (!Up)
		Algo::Reverse(out);
}

void AElevatorManager::GetTakenPendingGates(bool Up, TArray<int>& out) const
{
	out.Empty();
	for (auto Elevator : Elevators) 
		if (Elevator->State != ElevatorState::kStandby)
			out.Add(Elevator->GateIdxWhenStopped);
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
	//OnAnyArrival(GateIdx, ElevatorIdx);
}

void AElevatorManager::OnArrivalDown_Implementation(int GateIdx, int ElevatorIdx)
{
	//OnAnyArrival(GateIdx, ElevatorIdx);
}

void AElevatorManager::OnElevatorReadyToGo(int ElevatorIdx)
{
	bool MovingUp = Elevators[ElevatorIdx]->GetReasonOfMoving() == ElevatorState::kUp;
	int GateIdx = Elevators[ElevatorIdx]->GateIdxWhenStopped;
	((*Gates[GateIdx]).*(MovingUp ? &AGateBase::StartedUp : &AGateBase::StartedDown))();
	Schedule(ElevatorIdx, MovingUp, GateIdx);
}


// Called every frame
void AElevatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

