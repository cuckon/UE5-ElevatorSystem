#pragma once
#include "UObject/Class.h"

UENUM(BlueprintType)
enum class ElevatorState : uint8 {
	kUp UMETA(DisplayName = "Up"),
	kDown UMETA(DisplayName = "Down"),
	kStandby UMETA(DisplayName = "Standby"),

	// In the state of closing/opening doors
	kUnready UMETA(DisplayName = "Unready")
};
