// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/FindPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/BaseEnemy.h"
#include "Engine/TargetPoint.h"

EBTNodeResult::Type UFindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = OwnerController->GetPawn();

	if(!IsValid(ControlledPawn))
	{
		UE_LOG(LogTemp,Log,TEXT(""))
		return EBTNodeResult::Failed;
	}

	if(!IsValid(OwnerController))
	{
		UE_LOG(LogTemp,Log,TEXT(""))
		return EBTNodeResult::Failed;
	}
	
	ABaseEnemy* enemy = Cast<ABaseEnemy>(ControlledPawn);
	ATargetPoint* point = enemy->GetCurrentPatrolPoint();

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValue<UBlackboardKeyType_Vector>(BlackboardLocationKey.SelectedKeyName,
		point->GetActorLocation());
	
	return EBTNodeResult::Succeeded;
}
