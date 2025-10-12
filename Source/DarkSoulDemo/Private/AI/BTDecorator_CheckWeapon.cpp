// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckWeapon.h"

#include "AIController.h"
#include "Component/CharacterCombat.h"

bool UBTDecorator_CheckWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//这个地方拿到的是Controoler
	AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
	if(!IsValid(OwnerController))return false;

	APawn* OwnerPawn = OwnerController->GetPawn();
	if(!IsValid(OwnerPawn))return false;
	
	UCharacterCombat* CharacterCombat = OwnerPawn->GetComponentByClass<UCharacterCombat>();
	if(!IsValid(CharacterCombat))return false;

	return CharacterCombat->CanEnableCombat();
}
