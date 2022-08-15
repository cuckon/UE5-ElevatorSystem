// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElevatorManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELEVATOR_API UElevatorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElevatorManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<AActor*> Gates;
	TArray<AActor*> Elevators;
	TMap<const AActor*, float> HeightByGate;

	void SetupGates();
	void SetupElevators();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
