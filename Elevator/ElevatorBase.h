// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorManagable.h"
#include "ElevatorCommon.h"
#include "ElevatorBase.generated.h"


class UEaseMoveComponent;
class AElevatorManager;


UCLASS()
class ELEVATOR_API AElevatorBase : public AActor, public ElevatorManagable
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElevatorArrivalSignature, int, GateIdx, int, ElevatorIdx);

	// Sets default values for this actor's properties
	AElevatorBase();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		UEaseMoveComponent* EaseMove;

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		ElevatorState GetState() const { return State; }

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		ElevatorState GetReasonOfMoving() const { return ReasonOfMoving; }

	// Move to `NewTargetGateIdx`, switch to `Reason` when arrived.
	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void MoveToGate(int NewTargetGateIdx, ElevatorState Reason);



	FElevatorArrivalSignature ArrivalUpDelegates, ArrivalDownDelegates;
	int TargetGateIdx = -1;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnArrivial();

	void OnArrivial_Implementation();
	virtual int GetIdxInManager() override;

	ElevatorState State = ElevatorState::kStopped;

	ElevatorState ReasonOfMoving;  // Switch to this state after arrival

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
