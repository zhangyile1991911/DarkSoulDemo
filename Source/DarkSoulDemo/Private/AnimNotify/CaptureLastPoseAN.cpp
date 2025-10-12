// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/CaptureLastPoseAN.h"

#include "Component/CharacterStats.h"

void UCaptureLastPoseAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(!IsValid(MeshComp))
	{
		return;	
	}
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if(!IsValid(AnimInstance))
	{
		return;
	}

	AnimInstance->SavePoseSnapshot(FName("DeathPose"));

	// AActor* OwnerActor = MeshComp->GetOwner();
	// if(OwnerActor == nullptr)return;
	//
	// UCharacterStats* CharacterStats = OwnerActor->GetComponentByClass<UCharacterStats>();
	// if(CharacterStats == nullptr)return;
	//
	// CharacterStats->TakeDamage(99999999);
}
