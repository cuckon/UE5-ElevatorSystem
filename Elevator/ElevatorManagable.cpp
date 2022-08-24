#include "ElevatorManagable.h"

#include <Logging/LogMacros.h>
#include "ElevatorManager.h"

PRAGMA_DISABLE_OPTIMIZATION

void ElevatorManagable::SetManager(AElevatorManager* NewManager)
{
    Manager = NewManager;
}

bool ElevatorManagable::InitManagable()
{
	if (!Manager) {
		UE_LOG(LogInit, Error, TEXT("Manager is not set"));
		return false;
	}
	IdxInManager = GetIdxInManager();
	
	return true;
}
