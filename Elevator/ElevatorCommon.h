#pragma once
#include "UObject/Class.h"

UENUM(BlueprintType)
enum class ElevatorState : uint8 {
	kUp UMETA(DisplayName = "Up"),
	kDown UMETA(DisplayName = "Down"),
	kStopped UMETA(DisplayName = "Stopped")
};
