#pragma once

UENUM(BlueprintType)
enum class EWeaponCollisionType :uint8
{
	MainWeapon UMETA(Display="MainWeapon"),
	RightHand UMETA(Display="RightHand"),
	LeftHand UMETA(Display="LeftHand")
};