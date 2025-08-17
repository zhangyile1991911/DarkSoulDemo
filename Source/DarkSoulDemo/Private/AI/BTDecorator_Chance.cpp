// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_Chance.h"

bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int r = FMath::RandRange(1,100);
	return Chance > r;
}
