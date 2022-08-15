#include <Logging/LogMacros.h>
#include "ElevatorManagable.h"
#include "ElevatorManager.h"

void ElevatorManagable::SetManager(AElevatorManager* NewManager)
{
    Manager = NewManager;
}

void ElevatorManagable::BeginPlay()
{
	if (!Manager) {
		UE_LOG(LogInit, Error, TEXT("Manager is not set"));
		return;
	}
	IdxInManager = GetIdxInManager();
}
