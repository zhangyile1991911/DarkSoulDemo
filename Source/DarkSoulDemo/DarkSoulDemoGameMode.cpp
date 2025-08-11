// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkSoulDemoGameMode.h"
#include "DarkSoulDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADarkSoulDemoGameMode::ADarkSoulDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
