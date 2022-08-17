// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorManagable.h"
#include "GateBase.generated.h"


UCLASS()
class ELEVATOR_API AGateBase : public AActor, public ElevatorManagable
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParams(FGateStateChangeSignature, int, GateIdx);

	// Sets default values for this actor's properties
	AGateBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void OnPendingUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void OnPendingDown();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void StartedUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void StartedDown();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		bool IsPendingUp = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		bool IsPendingDown = false;

	FGateStateChangeSignature PendingUpDelegates;
	FGateStateChangeSignature PendingDownDelegates;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnPendingUp_Implementation();
	void OnPendingDown_Implementation();
	void StartedUp_Implementation();
	void StartedDown_Implementation();

	virtual int GetIdxInManager() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
