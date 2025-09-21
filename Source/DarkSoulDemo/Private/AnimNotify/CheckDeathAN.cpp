// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/CheckDeathAN.h"

#include "Component/CharacterStats.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UCheckDeathAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* OwnerActor = MeshComp->GetOwner();
	if(!IsValid(OwnerActor))return;

	UCharacterStats* CharacterStats = OwnerActor->GetComponentByClass<UCharacterStats>();
	if(!IsValid(CharacterStats))return;
	
	ACharacter* Character = Cast<ACharacter>(OwnerActor);
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if(!IsValid(AnimInstance))return;

	if(CharacterStats->IsDead())
	{
		AnimInstance->StopAllMontages(false);
	}
}
