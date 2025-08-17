// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_InRangeCheck.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_InRangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	if(blackBoard == nullptr)return false;

	UObject* obj = blackBoard->GetValueAsObject(TargetKeySelector.SelectedKeyName);
	if(obj == nullptr)return false;
	
	AActor* target = Cast<AActor>(obj);
	if(target == nullptr)return false;

	AActor* owner = OwnerComp.GetOwner();
	AController* Controller = Cast<AController>(owner);
	if(Controller == nullptr)return false;
	
	float distance = Controller->GetPawn()->GetDistanceTo(target);
	UE_LOG(LogTemp,Log,TEXT("InRangeCheck distance = %f"),distance)
	if(distance >= LessThan && distance <= GreaterThan)
	{
		return true;
	}
	return false;
}
