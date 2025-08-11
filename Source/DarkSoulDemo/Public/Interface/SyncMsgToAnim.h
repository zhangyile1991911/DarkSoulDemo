// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SyncMsgToAnim.generated.h"

enum class ECombatType : uint8;
// This class does not need to be modified.
UINTERFACE()
class USyncMsgToAnim : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOULDEMO_API ISyncMsgToAnim
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interact")
	void SyncCombatMode(ECombatType combatType);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interact")
	void SyncBlocking(bool bIsBlocking);
};
