// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/UnequipAN.h"

#include "Component/CharacterCombat.h"
#include "Equipment/BaseWeapon.h"

void UUnequipAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if(!IsValid(Actor))
	{
		UE_LOG(LogTemp,Log,TEXT(""))
		return;
	}

	UCharacterCombat* combat = Actor->GetComponentByClass<UCharacterCombat>();
	if(!IsValid(combat))
	{
		UE_LOG(LogTemp,Log,TEXT(""))
		return;
	}

	TObjectPtr<ABaseWeapon> weaponPtr = combat->GetMainWeapon();
	if(!weaponPtr)
	{
		UE_LOG(LogTemp,Log,TEXT(""))
		return;
	}

	weaponPtr->UnequipItem();
}
