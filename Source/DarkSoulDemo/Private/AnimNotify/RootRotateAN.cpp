// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/RootRotateAN.h"

#include "Component/CharacterRotator.h"

void URootRotateAN::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if(Owner == nullptr)return;

	UCharacterRotator* CharacterRotator = Owner->GetComponentByClass<UCharacterRotator>();
	if(CharacterRotator == nullptr)return;

	CharacterRotator->ToggleRotation(true);
}

void URootRotateAN::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	
}

void URootRotateAN::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if(Owner == nullptr)return;

	UCharacterRotator* CharacterRotator = Owner->GetComponentByClass<UCharacterRotator>();
	if(CharacterRotator == nullptr)return;

	CharacterRotator->ToggleRotation(false);
}
