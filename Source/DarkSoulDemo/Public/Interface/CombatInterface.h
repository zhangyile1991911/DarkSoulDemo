// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EWeaponCollisionType.h"
#include "ECombatType.h"
#include "CombatInterface.generated.h"

enum class EMontageAction : uint8;
// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOULDEMO_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ActivateCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	virtual void DeactiveCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	virtual float PerformAttack(EMontageAction AttackType) = 0;
	virtual ECombatType CombatType() = 0;
	virtual void Parried(AActor* Actor) = 0;
	virtual void StealthKilled() = 0;
	virtual bool CanBeStealthKilled() = 0;
	virtual void RiposteKilled() = 0;
	virtual bool CanBeRiposteKilled() = 0;
	
};
