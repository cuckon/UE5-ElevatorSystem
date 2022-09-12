// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagableBase.h"
#include "GateBase.generated.h"


UCLASS(ClassGroup = Elevator)
class ELEVATOR_API AGateBase : public AManagableBase
{
	GENERATED_BODY()
	
public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGateStateChangeSignature, int, GateIdx, bool, Up);

	// Sets default values for this actor's properties
	AGateBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void OnPending(bool Up);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Elevator")
		void OnPicked(bool Up);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		bool IsPendingUp = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		bool IsPendingDown = false;

	FGateStateChangeSignature PendingDelegates;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnPending_Implementation(bool Up);
	void OnPicked_Implementation(bool Up);
	virtual int GetIdxInManager() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
