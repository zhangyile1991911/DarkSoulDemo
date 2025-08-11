#pragma once

UENUM(BlueprintType)
enum class ECollisionType : uint8
{
	None UMETA(Displaynam="None"),
	MainWeapon UMETA(Displaynam="MainWeapon"),
	SecondWeapon UMETA(Displaynam="SecondWeapon"),
};
