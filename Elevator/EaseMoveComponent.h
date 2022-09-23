// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EaseMoveComponent.generated.h"


UCLASS( ClassGroup=Movement, meta=(BlueprintSpawnableComponent) )
class ELEVATOR_API UEaseMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArrivalSignature);

	// Sets default values for this component's properties
	UEaseMoveComponent();

	UFUNCTION(BlueprintCallable, Category = "Elevator")
		void MoveTo(const FVector& Position);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		float Speed = 200.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		int LagSteps = 30;

	UPROPERTY(BlueprintAssignable, Category = "Elevator")
		FArrivalSignature ArrivalDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Elevator")
		FArrivalSignature BeginMoveDelegate;


	UPROPERTY(BlueprintReadOnly, Category = "Elevator")
		bool IsArrived = true;

	UPROPERTY(BlueprintReadOnly, Category = "Elevator")
		float CurrentSpeed = 0.0;

	FVector GetIntermediatePosition() const { return IntermediatePosition; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool MoveX = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool MoveY = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool MoveZ = true;


	// DEBUG helpers
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool DrawIntermediatePosition = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool DrawTargetPosition = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector TargetPosition;
	FVector IntermediatePosition;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
