// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KnockBackAN.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UKnockBackAN : public UAnimNotify
{
	GENERATED_BODY()
public:
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	// float BaseDamage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FName LocationSocket;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	float DamageRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	TSubclassOf<UDamageType> DamageTypeClass;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
