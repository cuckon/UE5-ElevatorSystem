// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagableBase.generated.h"

class AElevatorManager;

UCLASS(Abstract, ClassGroup = Elevator)
class ELEVATOR_API AManagableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManagableBase();
	
	UFUNCTION(BlueprintCallable)
		void SetManager(AElevatorManager* NewManager);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintSetter = SetManager)
		AElevatorManager* Manager = nullptr;
	int IdxInManager;

	UFUNCTION()
		virtual int GetIdxInManager()
		PURE_VIRTUAL(AManagableBase::GetIdxInManager, return -1;);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
