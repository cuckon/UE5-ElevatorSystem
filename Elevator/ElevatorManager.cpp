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
	PrimaryActorTick.bCanEverTick = false;

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
		float AHeight = A ? A->GetActorLocation().Z : -99999;
		float BHeight = B ? B->GetActorLocation().Z : -99999;
		return AHeight < BHeight;
		});

	for (auto Elevator : Elevators) {
		if (!Elevator) continue;

		Elevator->Manager = this;
		Elevator->ArrivalDelegates.AddDynamic(this, &AElevatorManager::OnArrival);
		Elevator->ReadyToGoDelegates.AddDynamic(this, &AElevatorManager::OnElevatorReadyToGo);
	}
	for (auto Gate : Gates) {
		Gate->Manager = this;
		Gate->PendingDelegates.AddDynamic(this, &AElevatorManager::OnPending);
	}
	UE_LOG(LogInit, Log, TEXT("PreInitializeComponents"));
}

// Called when the game starts or when spawned
void AElevatorManager::BeginPlay()
{
	Super::BeginPlay();
}


void AElevatorManager::OnPending_Implementation(int GateIdx, bool IsUp) {
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
	// 1. See if there's elevators happen to be at that gate and haven't shut the door completely.
	for (int i = 0; i < Elevators.Num(); i++)
		if (Elevators[i] && Elevators[i]->State == ElevatorState::kUnready
			&& Elevators[i]->GateIdxWhenStopped==GateIdx)
			return i;

	// 2. Try to find nearest standby elevators.
	TArray<int> ElevatorIndices;
	for (int i = 0; i < Elevators.Num(); i++)
		if (Elevators[i] && Elevators[i]->State == ElevatorState::kStandby)
			ElevatorIndices.Add(i);

	if (!ElevatorIndices.IsEmpty())
		return NearestElevatorToGate(GateIdx, ElevatorIndices);

	// 3. Try to finds nearest elevators that can pick up the gate.
	for (int i = 0; i < Elevators.Num(); i++) {
		if (!Elevators[i])
			continue;

		auto Elevator = Elevators[i];

		// Check if it's manipulating its doors.
		if (Elevator->State == ElevatorState::kUnready)
			continue;

		// Check if can pick it up on its way. i.e: Elevator is between intermediate and old target gate.
		if (ForUp != (Elevator->State == ElevatorState::kUp))
			continue;

		float IntermediateHeight = Elevator->EaseMove->GetIntermediatePosition().Z;
		float TargetHeight = Gates[Elevator->GateIdxWhenStopped]->GetActorLocation().Z;
		float GateHeight = Gates[GateIdx]->GetActorLocation().Z;
		if (ForUp && IntermediateHeight < GateHeight && GateHeight < TargetHeight 
			|| !ForUp && IntermediateHeight > GateHeight && GateHeight > TargetHeight)
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
		if (!Gates[i])
			continue;
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
		if (Elevator && Elevator->State != ElevatorState::kStandby)
			out.Add(Elevator->GateIdxWhenStopped);
}

void AElevatorManager::OnArrival_Implementation(int GateIdx, int ElevatorIdx, bool ArrivalForUp)
{
	Gates[GateIdx]->OnPicked(ArrivalForUp);
}

void AElevatorManager::OnElevatorReadyToGo(int ElevatorIdx)
{
	bool MovingUp = Elevators[ElevatorIdx]->GetReasonOfMoving() == ElevatorState::kUp;
	int GateIdx = Elevators[ElevatorIdx]->GateIdxWhenStopped;
	Schedule(ElevatorIdx, MovingUp, GateIdx);
}

// Called every frame
void AElevatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

