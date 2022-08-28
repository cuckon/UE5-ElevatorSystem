// Fill out your copyright notice in the Description page of Project Settings.


#include "GateBase.h"
#include "ElevatorManager.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
AGateBase::AGateBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

 //Called when the game starts or when spawned
void AGateBase::BeginPlay()
{
	Super::BeginPlay();
	InitManagable();
}

void AGateBase::OnPendingUp_Implementation()
{
	IsPendingUp = true;
	PendingUpDelegates.Broadcast(IdxInManager);
}

void AGateBase::OnPendingDown_Implementation()
{
	IsPendingDown = true;
	PendingDownDelegates.Broadcast(IdxInManager);
}

void AGateBase::StartedUp_Implementation()
{
	IsPendingUp = false;
}

void AGateBase::StartedDown_Implementation()
{
	IsPendingDown = false;
}

int AGateBase::GetIdxInManager()
{
	return Manager->Gates.Find(this);;
}

// Called every frame
void AGateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

