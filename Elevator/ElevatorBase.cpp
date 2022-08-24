// Fill out your copyright notice in the Description page of Project Settings.
#include "ElevatorBase.h"

#include <VectorTypes.h>
#include "EaseMoveComponent.h"
#include "GateBase.h"
#include "ElevatorManager.h"


// Sets default values
AElevatorBase::AElevatorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EaseMove = CreateDefaultSubobject<UEaseMoveComponent>(TEXT("EaseMove"));
	AddOwnedComponent(EaseMove);
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}


// Called when the game starts or when spawned
void AElevatorBase::BeginPlay()
{
	Super::BeginPlay();
	InitManagable();
	EaseMove->ArrivalDelegate.AddUniqueDynamic(this, &AElevatorBase::OnArrivial);
}


void AElevatorBase::OnArrivial_Implementation() {
	FElevatorArrivalSignature ArrivalDelegates = State == ElevatorState::kUp ? ArrivalUpDelegates : ArrivalDownDelegates;
	ArrivalDelegates.Broadcast(TargetGateIdx, IdxInManager);
	State = ElevatorState::kStopped;
}


void AElevatorBase::MoveToGate(int NewTargetGateIdx) {
	TargetGateIdx = NewTargetGateIdx;

	FVector TargetPos = GetActorLocation();
	float CurrentHeight = TargetPos.Z;
	TargetPos.Z = Manager->Gates[TargetGateIdx]->GetActorLocation().Z;
	EaseMove->MoveTo(TargetPos);

	if (TargetPos.Z > CurrentHeight)
		State = ElevatorState::kUp;
	else if (TargetPos.Z > CurrentHeight)
		State = ElevatorState::kDown;
	else
		State = ElevatorState::kStopped;
}


int AElevatorBase::GetIdxInManager()
{
	return Manager->Elevators.Find(this);
}


// Called every frame
void AElevatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

