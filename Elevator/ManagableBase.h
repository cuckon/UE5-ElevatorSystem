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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elevator")
		AElevatorManager* Manager = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual int GetIdxInManager() const
		PURE_VIRTUAL(AManagableBase::GetIdxInManager, return -1;);

	int IdxInManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
