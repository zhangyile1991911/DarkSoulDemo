#pragma once
#include "EHitDirection.generated.h"

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	None UMETA(Displayname = "None"),
	FRONT UMETA(Displayname = "Front"),
	BACK UMETA(Displayname = "Back"),
	LEFT UMETA(Displayname = "Left"),
	RIGHT UMETA(Displayname = "Right")
};

inline FName HitDirectionDisplayName(EHitDirection HitDirection)
{
	const UEnum* EnumPtr = StaticEnum<EHitDirection>();
	if(!EnumPtr)
	{
		return NAME_None;
	}
	return FName(EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(HitDirection)).ToString());
}