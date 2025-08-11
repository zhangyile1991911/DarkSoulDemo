// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ResetMovementInput.h"

#include "Component/CharacterState.h"

void UResetMovementInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Actor = MeshComp->GetOwner();
	if(!IsValid(Actor))
	{
		UE_LOG(LogTemp,Log,TEXT("UResetMovementInput GetOwner == nullptr"))
		return;
	}

	UCharacterState* StateComponent = Actor->GetComponentByClass<UCharacterState>();
	if(!IsValid(StateComponent))
	{
		UE_LOG(LogTemp,Log,TEXT("UResetMovementInput CharacterState == nullptr"))
		return;
	}

	StateComponent->EnableMovement();
}
