// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagableBase.h"

// Sets default values
AManagableBase::AManagableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AManagableBase::SetManager(AElevatorManager* NewManager)
{
	Manager = NewManager;
	UE_LOG(LogInit, Log, TEXT("void AManagableBase::SetManager(AElevatorManager* NewManager)"));
}

// Called when the game starts or when spawned
void AManagableBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Manager) {
		UE_LOG(LogInit, Fatal, TEXT("Manager is not set"));
		return;
	}
	IdxInManager = GetIdxInManager();	
}

// Called every frame
void AManagableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

