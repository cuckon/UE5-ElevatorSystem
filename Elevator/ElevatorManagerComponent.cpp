// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorManagerComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UElevatorManagerComponent::UElevatorManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//UClass* GetBpClass(const FString & Name)
//{
//	UClass* Cls =;
//	
//	
//	if (Cls.IsValid()) {
//		return Cls.LoadSynchronous();
//	}
//	return nullptr;
//}
//


// Called when the game starts
void UElevatorManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupGates();
	
	SetupElevators();
}


void UElevatorManagerComponent::SetupGates()
{
	UClass* GateCls = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/BPs/Gate/BP_Gate.BP_Gate_C'"));
	if (!GateCls) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find BP_Gate!"));
		return;
	}

	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), GateCls, Gates);
	Algo::Sort(Gates, [](const AActor* A, const AActor* B) {
		return A->GetActorLocation().Z < B->GetActorLocation().Z;
		}
	);

	for (auto Gate : Gates) {
		HeightByGate.Add(Gate, Gate->GetActorLocation().Z);
	}
}

void UElevatorManagerComponent::SetupElevators()
{
	UClass* GateCls = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/BPs/Gate/BP_Gate.BP_Gate_C'"));
	if (!GateCls) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find BP_Gate!"));
		return;
	}

	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), GateCls, Elevators);
}

// Called every frame
void UElevatorManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

