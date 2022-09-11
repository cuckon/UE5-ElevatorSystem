// Fill out your copyright notice in the Description page of Project Settings.


#include "GateBase.h"
#include "ElevatorManager.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
AGateBase::AGateBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

 //Called when the game starts or when spawned
void AGateBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGateBase::OnPending_Implementation(bool Up)
{
	(Up ? IsPendingUp : IsPendingDown) = true;
	PendingDelegates.Broadcast(IdxInManager, Up);
}

int AGateBase::GetIdxInManager() const
{
	return Manager->Gates.Find(const_cast<AGateBase*>(this));
}

// Called every frame
void AGateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

