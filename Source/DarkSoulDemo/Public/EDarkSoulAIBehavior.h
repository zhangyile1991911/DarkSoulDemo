#pragma once

UENUM(BlueprintType)
enum class EDarkSoulAIBehavior : uint8
{
	None UMETA(DisplayName="None"),
	Idle UMETA(DisplayName="Idle"),
	Patrol UMETA(DisplayName="Patrol"),
	MeleeAttack UMETA(DisplayName="MeleeAttack"),
	Approach  UMETA(DisplayName="Approach"),
	Stunned UMETA(DisplayName="Stunned"),
	Strafe UMETA(DisplayName="Strafe"),
};
