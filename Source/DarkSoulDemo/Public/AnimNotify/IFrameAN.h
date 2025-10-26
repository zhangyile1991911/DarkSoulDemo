// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "IFrameAN.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UIFrameAN : public UAnimNotifyState
{
	GENERATED_BODY()

	// ECollisionResponse OriginCollisionResponse;
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
