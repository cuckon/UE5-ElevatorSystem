// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagableBase.h"
#include "ElevatorCommon.h"
#include "ElevatorBase.generated.h"


class UEaseMoveComponent;
class AElevatorManager;


UCLASS(ClassGroup=Elevator)
class ELEVATOR_API AElevatorBase : public AManagableBase
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElevatorArrivalSignature, int, GateIdx, int, ElevatorIdx);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElevatorReadyToGoSignature, int, ElevatorIdx);

	// Sets default values for this actor's properties
	AElevatorBase();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		UEaseMoveComponent* EaseMove;

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		ElevatorState GetReasonOfMoving() const { return ReasonOfMoving; }

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void ReadyToGo() const { ReadyToGoDelegates.Broadcast(GetIdxInManager()); UE_LOG(LogInit, Log, TEXT("Ready To go: %d."), GateIdxWhenStopped); }

	// Move to `NewTargetGateIdx`, switch to `Reason` when arrived.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Elevator")
		void MoveToGate(int NewTargetGateIdx, ElevatorState Reason);

	UPROPERTY(BlueprintAssignable, Category = "Elevator")
		FElevatorArrivalSignature ArrivalUpDelegates;

	UPROPERTY(BlueprintAssignable, Category = "Elevator")
		FElevatorArrivalSignature ArrivalDownDelegates;

	UPROPERTY(BlueprintAssignable, Category = "Elevator")
		FElevatorReadyToGoSignature ReadyToGoDelegates;

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnArrivial();

	UPROPERTY(BlueprintReadWrite, Category = "Elevator")
		ElevatorState State = ElevatorState::kStandby;


	int GateIdxWhenStopped = -1;


	void MoveToGate_Implementation(int NewTargetGateIdx, ElevatorState Reason);
	void OnArrivial_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual int GetIdxInManager() const override;

	ElevatorState ReasonOfMoving;  // Switch to this state after arrival

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
