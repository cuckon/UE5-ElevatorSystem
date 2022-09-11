// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagableBase.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
AManagableBase::AManagableBase()
{
}

// Called when the game starts or when spawned
void AManagableBase::BeginPlay()
{
	Super::BeginPlay();
	if (Manager) {
		IdxInManager = GetIdxInManager();
	}
	else if (GetWorld()->IsGameWorld()) {
		UE_LOG(LogInit, Warning, TEXT("Manager is not set"));
	}
}

// Called every frame
void AManagableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

