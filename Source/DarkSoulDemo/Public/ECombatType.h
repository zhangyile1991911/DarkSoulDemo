#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None UMETA(DisplayName = "None"),
	SingleSword UMETA(Display = "SingleSword"),
	SwordShield UMETA(Display="SowrdShield"),
	TwoHand UMETA(Display="TwoHand"),
	Fists UMETA(Display="Fists")
};
