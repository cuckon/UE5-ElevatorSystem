// Fill out your copyright notice in the Description page of Project Settings.


#include "EaseMoveComponent.h"

PRAGMA_DISABLE_OPTIMIZATION

const float kSmallDistance = 0.1;

// Sets default values for this component's properties
UEaseMoveComponent::UEaseMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UEaseMoveComponent::MoveTo(const FVector& Position)
{
	if (IsArrived)
		BeginMoveDelegate.Broadcast();
	IsArrived = false;

	TargetPosition = Position;
	
}

// Called when the game starts
void UEaseMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Log, TEXT("Speed: %f, LagS: %d"), Speed, LagSteps);
	
}


// Called every frame
void UEaseMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentSpeed = 0.0f;

	if (IsArrived)
		return;

	// ...

	FVector CurrentPos = GetOwner()->GetActorLocation();

	// Update intermediate
	float DistanceToGo = FVector::Distance(CurrentPos, TargetPosition);
	if (DistanceToGo < kSmallDistance) {
		IsArrived = true;
		CurrentSpeed = DistanceToGo / DeltaTime;
		
		ArrivalDelegate.Broadcast();

		return;
	}

	if (FVector::Distance(IntermediatePosition, TargetPosition) < Speed * DeltaTime) {
		IntermediatePosition = TargetPosition;
	}
	else {
		IntermediatePosition += (TargetPosition - IntermediatePosition).GetUnsafeNormal() * Speed * DeltaTime;
	}


	// Update position
	FVector DeltaMove = (IntermediatePosition - CurrentPos) / LagSteps;
	GetOwner()->SetActorLocation(CurrentPos + DeltaMove);
	CurrentSpeed = DeltaMove.Length() / DeltaTime;

	if (DrawIntermediatePosition)
		DrawDebugBox(GetWorld(), IntermediatePosition, FVector(100), FColor::Green);
}

