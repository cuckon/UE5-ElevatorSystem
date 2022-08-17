// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorManager.generated.h"

class AElevatorBase;
class AGateBase;


UCLASS()
class ELEVATOR_API AElevatorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		TArray<AElevatorBase*> Elevators;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		TArray<AGateBase*> Gates;


protected:
	void Schedule(AElevatorBase* Elevator);

	virtual void BeginPlay() override;

	USceneComponent* SceneComponent;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnAnyArrival(int GateIdx, int ElevatorIdx);

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnAnyPending(int GateIdx);

	void OnAnyArrival_Implementation(int, int);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
