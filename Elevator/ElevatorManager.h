// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorCommon.h"
#include "ElevatorManager.generated.h"

class AElevatorBase;
class AGateBase;


UCLASS(ClassGroup = Elevator)
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

	UFUNCTION(CallInEditor, Category = "Elevator")
		void AddAllGates();

	UFUNCTION(CallInEditor, Category = "Elevator")
		void AddAllElevators();

	void PreInitializeComponents() override;

protected:
	//void Schedule(AElevatorBase* Elevator, int GateIdx, bool IsUp);

	virtual void BeginPlay() override;

	USceneComponent* SceneComponent;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnArrivalUp(int GateIdx, int ElevatorIdx);

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnArrivalDown(int GateIdx, int ElevatorIdx);

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnPendingUp(int GateIdx);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnPendingDown(int GateIdx);

	// Move to next proper gate. While be consistent with current moving direction.
	// SplitGateIdx : Should be the gate that scan
	void Schedule(int ElevatorIdx, bool PreferUp, int SplitGateIdx);

	void OnArrivalUp_Implementation(int, int);
	void OnArrivalDown_Implementation(int, int);
	void OnAnyArrival(int, int);
	void OnPendingUp_Implementation(int);
	void OnPendingDown_Implementation(int);
	void OnAnyPending(bool IsUp, int GateIdx);
	int BestElevatorForPendingGate(int GateIdx, bool ForUp) const;
	void GetUntakenPendingGates(bool Up, TArray<int>& out) const;
	void GetTakenPendingGates(bool Up, TArray<int>& out) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
