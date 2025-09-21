#pragma once

UENUM(BlueprintType)
enum class EDamageType: uint8
{
	None UMETA(DisplayName ="None"),
	HitBack UMETA(DisplayName="HitBack"),
	KnockBack UMETA(DisplayName="KnockBack")
};
