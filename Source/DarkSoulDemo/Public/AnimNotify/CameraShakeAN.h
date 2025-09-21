// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CameraShakeAN.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UCameraShakeAN : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float InnerRadius = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float OuterRadius = 1000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Falloff = 1.f;
};
