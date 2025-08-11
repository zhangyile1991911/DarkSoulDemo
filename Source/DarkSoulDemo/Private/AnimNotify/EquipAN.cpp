// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EquipAN.h"

#include "Component/CharacterCombat.h"

#include "Equipment/BaseWeapon.h"

void UEquipAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if(!IsValid(Owner))
	{
		UE_LOG(LogTemp,Log,TEXT("UEquipAN::Notify Owner is invalid"))
		return;
	}
	
	UCharacterCombat* Combat = Owner->GetComponentByClass<UCharacterCombat>();
	if(!IsValid(Combat))
	{
		UE_LOG(LogTemp,Log,TEXT("UEquipAN::Notify UCharacterCombat is invalid"))
		return;
	}
	Combat->EnableCombat();
	
	TObjectPtr<ABaseWeapon> MainWeapon = Combat->GetMainWeapon();
	if(!MainWeapon)
	{
		UE_LOG(LogTemp,Log,TEXT("UEquipAN::Notify ABaseWeapon is invalid"))
		return;
	}
	MainWeapon->AttachToPlayer(MainWeapon->GetEquippedSocket());
}
