// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponCollisionType.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WeaponCollisionAN.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UWeaponCollisionAN : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EWeaponCollisionType WeaponCollisionType = EWeaponCollisionType::MainWeapon;
public:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
