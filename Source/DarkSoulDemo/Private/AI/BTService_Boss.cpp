// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Boss.h"

#include "AIController.h"
#include "EDarkSoulAIBehavior.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseEnemy.h"
#include "Component/CharacterStats.h"

void UBTService_Boss::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	SetOwner(OwnerComp.GetOwner());
}

void UBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UpdateBehavior();
}

void UBTService_Boss::SetOwner(AActor* ActorOwner)
{
	Super::SetOwner(ActorOwner);
	AIController = Cast<AAIController>(ActorOwner);
	BaseEnemy = Cast<ABaseEnemy>(AIController->GetPawn());
}

void UBTService_Boss::UpdateBehavior()
{
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if(BlackboardComponent == nullptr)return;

	UCharacterStats* StatsComp = BaseEnemy->GetComponentByClass<UCharacterStats>();
	if(StatsComp == nullptr)return;

	UObject* obj = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	if(obj == nullptr)return;
	
	ACharacter* TargetCharacter = Cast<ACharacter>(obj);
	if(!IsValid(TargetCharacter))return;
	
	
	float baseStamina = StatsComp->GetBaseStamina();
	bool bCanStrafe = baseStamina <= BossStaminaCheck;
	if(bCanStrafe)
	{
		BlackboardComponent->SetValueAsEnum(BehaviourKey.SelectedKeyName,
			static_cast<uint8>(EDarkSoulAIBehavior::Strafe));
		return;
	}

	float localDistance = FVector::Distance(BaseEnemy->GetActorLocation(),
		TargetCharacter->GetActorLocation());

	if(localDistance <= AttackRangeDistance)
	{
		BlackboardComponent->SetValueAsEnum(
			BehaviourKey.SelectedKeyName,
			static_cast<uint8>(EDarkSoulAIBehavior::MeleeAttack));
	}
	else
	{
		BlackboardComponent->SetValueAsEnum(
			BehaviourKey.SelectedKeyName,
			static_cast<uint8>(EDarkSoulAIBehavior::Approach));
	}
}

UBTService_Boss::UBTService_Boss()
{
	bNotifyBecomeRelevant = true;
	AttackRangeDistance = 250.0f;
}
