// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorManagable.h"
#include "ElevatorBase.generated.h"


class UEaseMoveComponent;
class AElevatorManager;


UENUM(BlueprintType)
enum class ElevatorState : uint8 {
	kUp UMETA(DisplayName = "Up"),
	kDown UMETA(DisplayName = "Down"),
	kStopped UMETA(DisplayName = "Stopped")
};


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
		ElevatorState GetState() const;

	//void SetManager(AElevatorManager*);
	void StartNextMove();

	
	TArray<int> Roadmap;
	FElevatorArrivalSignature ArrivalDelegates;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Elevator")
		void OnArrivial();

	void OnArrivial_Implementation();
	
	virtual int GetIdxInManager() override;

	//AElevatorManager* Manager = nullptr;
	//int IdxInManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
