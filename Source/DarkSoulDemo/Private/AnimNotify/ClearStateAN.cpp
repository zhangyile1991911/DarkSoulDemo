// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ClearStateAN.h"

#include "Component/CharacterState.h"

void UClearStateAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Actor = MeshComp->GetOwner();
	if(!IsValid(Actor))
	{
		UE_LOG(LogTemp,Log,TEXT("UClearStateAN::Notify Owner == nullptr"))
		return;
	}

	UCharacterState* CharacterState = Actor->GetComponentByClass<UCharacterState>();
	if(!IsValid(CharacterState))
	{
		UE_LOG(LogTemp,Log,TEXT("UClearStateAN::Notify "))
		return;
	}

	CharacterState->RecoverToNormalState();
}
