// Fill out your copyright notice in the Description page of Project Settings.

#include <VectorTypes.h>
#include "ElevatorBase.h"
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

ElevatorState AElevatorBase::GetState() const
{
	return ElevatorState::kStopped;
}


// Called when the game starts or when spawned
void AElevatorBase::BeginPlay()
{
	Super::BeginPlay();
	EaseMove->ArrivalDelegate.AddUniqueDynamic(this, &AElevatorBase::OnArrivial);
}

void AElevatorBase::OnArrivial_Implementation() {
	int ArrivedGateIdx = Roadmap[0];
	Roadmap.RemoveAt(0);
	ArrivalDelegates.Broadcast(ArrivedGateIdx, IdxInManager);


	// TODO: begin after cloing the door
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AElevatorBase::StartNextMove, 1.0, false);
}

int AElevatorBase::GetIdxInManager()
{
	return Manager->Elevators.Find(this);
}

void AElevatorBase::StartNextMove()
{
	if (Roadmap.IsEmpty()) {
		UE_LOG(
			LogTemp, Log, TEXT("Calling StartNextMove while Roadmap is empty: %s"),
			*GetActorLabel()
		);
		return;
	}
	FVector TargetPos = GetActorLocation();
	TargetPos.Z = Manager->Gates[Roadmap[0]]->GetActorLocation().Z;
	EaseMove->MoveTo(TargetPos);

	UE_LOG(
		LogTemp, Log, TEXT("StartNextMove: %d"),
		Roadmap[0]
	);
}

// Called every frame
void AElevatorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

