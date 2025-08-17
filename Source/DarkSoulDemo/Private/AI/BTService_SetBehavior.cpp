// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SetBehavior.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseEnemy.h"
#include "GameFramework/Character.h"
#include "UI/EDarkSoulAIBehavior.h"

UBTService_SetBehavior::UBTService_SetBehavior()
{
	bNotifyBecomeRelevant = true;
	AttackRangeDistance = 250.0f;
}

void UBTService_SetBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	SetOwner(OwnerComp.GetOwner());
	
}

void UBTService_SetBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UpdateBehavior();
}

void UBTService_SetBehavior::SetOwner(AActor* ActorOwner)
{
	UE_LOG(LogTemp,Log,TEXT("UBTService_SetBehavior::SetOwner %s"),*ActorOwner->GetFName().ToString())
	AIController = Cast<AAIController>(ActorOwner);
	AIPawn = Cast<ACharacter>(AIController->GetPawn());
}

void UBTService_SetBehavior::UpdateBehavior()
{
	if(!IsValid(AIController))return;
	
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if(BlackboardComponent == nullptr)return;

	UObject* obj = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	if(IsValid(obj))
	{
		AActor* targetActor = Cast<AActor>(obj);
		if(IsValid(targetActor))
		{
			float curDistance = AIPawn->GetDistanceTo(targetActor);
			if(curDistance <= AttackRangeDistance)
			{
				BlackboardComponent->SetValueAsEnum(BehaviorKey.SelectedKeyName,uint8(EDarkSoulAIBehavior::MeleeAttack));	
			}
			else
			{
				BlackboardComponent->SetValueAsEnum(BehaviorKey.SelectedKeyName,uint8(EDarkSoulAIBehavior::Approach));	
			}
		}
	}
	else
	{
		ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(AIPawn);
		if(!IsValid(BaseEnemy))return;

		if(BaseEnemy->hasPatrolPoint())
		{
			BlackboardComponent->SetValueAsEnum(BehaviorKey.SelectedKeyName,uint8(EDarkSoulAIBehavior::Patrol));
		}
		else
		{
			BlackboardComponent->SetValueAsEnum(BehaviorKey.SelectedKeyName,uint8(EDarkSoulAIBehavior::Idle));
		}
	}
}
