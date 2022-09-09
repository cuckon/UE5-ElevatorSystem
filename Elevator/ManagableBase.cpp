// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagableBase.h"

// Sets default values
AManagableBase::AManagableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManagableBase::BeginPlay()
{
	Super::BeginPlay();
}

void AManagableBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (Manager) {
		IdxInManager = GetIdxInManager();
	}
	else if (GetWorld()->IsGameWorld()){
		UE_LOG(LogInit, Fatal, TEXT("Manager is not set"));
	}
}

// Called every frame
void AManagableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

