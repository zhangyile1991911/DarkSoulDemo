// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOULDEMO_API IInteract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//在cpp侧实现
	virtual void Interact(AActor* Actor) = 0;

	//可以在蓝图中覆写 或者 cpp中覆写
	//如果在cpp中覆写 必须 Interact_Implementation(AActor* Actor);
	// UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Interact")
	// void Interact(AActor* Actor);
};
