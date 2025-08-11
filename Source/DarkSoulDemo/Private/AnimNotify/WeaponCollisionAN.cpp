// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/WeaponCollisionAN.h"

#include "Interface/CombatInterface.h"

void UWeaponCollisionAN::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AActor* Owner =  MeshComp->GetOwner();
	if(!IsValid(Owner))
	{
		UE_LOG(LogTemp,Log,TEXT("UWeaponCollisionAN::NotifyBegin Owner == nullptr"))
		return;
	}
	bool bIsCombatInterface = Owner->GetClass()->ImplementsInterface(UCombatInterface::StaticClass());
	if(bIsCombatInterface)
	{
		ICombatInterface* interface = Cast<ICombatInterface>(Owner);
		interface->ActivateCollision(WeaponCollisionType);
	}
}

void UWeaponCollisionAN::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	AActor* Owner =  MeshComp->GetOwner();
	if(!IsValid(Owner))
	{
		UE_LOG(LogTemp,Log,TEXT("UWeaponCollisionAN::NotifyEnd Owner == nullptr"))
		return;
	}
	bool bIsCombatInterface = Owner->GetClass()->ImplementsInterface(UCombatInterface::StaticClass());
	if(bIsCombatInterface)
	{
		ICombatInterface* interface = Cast<ICombatInterface>(Owner);
		interface->DeactiveCollision(WeaponCollisionType);
	}
}
