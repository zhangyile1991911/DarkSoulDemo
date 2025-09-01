// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Stunned.h"

#include "AIController.h"
#include "Character/BaseEnemy.h"
#include "Component/CharacterState.h"


EBTNodeResult::Type UStunned::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	if(controller == nullptr)return EBTNodeResult::Succeeded;
	
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(controller->GetPawn());
	if(BaseEnemy == nullptr)return EBTNodeResult::Succeeded;

	UCharacterState* stateComp = BaseEnemy->GetComponentByClass<UCharacterState>();
	if(stateComp == nullptr) return EBTNodeResult::Succeeded;

	stateComp->AddState(Player_State_Stunned);

	GetWorld()->GetTimerManager().SetTimer(WaitFinishHandle,[this,&OwnerComp,stateComp]()
	{
		stateComp->RemoveState(Player_State_Stunned);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	},1.0f,false,StunnedDuration);
	return EBTNodeResult::InProgress;
}

void UStunned::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if(WaitFinishHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitFinishHandle);
	}
}
