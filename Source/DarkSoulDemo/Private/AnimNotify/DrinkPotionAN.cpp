// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/DrinkPotionAN.h"

#include "Component/PotionInventory.h"

void UDrinkPotionAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	AActor* Actor = MeshComp->GetOwner();
	if(!IsValid(Actor))return;

	UPotionInventory* PotionInventory = Actor->GetComponentByClass<UPotionInventory>();
	if(!IsValid(PotionInventory))return;

	PotionInventory->DrinkPotion();
}
