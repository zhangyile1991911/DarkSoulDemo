// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PerformAttack.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"

EBTNodeResult::Type UPerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	ACharacter* ControllerPawn = Cast<ACharacter>(OwnerController->GetPawn());

	bool bIsImplement = ControllerPawn->GetClass()->ImplementsInterface(UCombatInterface::StaticClass());
	if(!bIsImplement)
	{
		return EBTNodeResult::Succeeded;
	}
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(ControllerPawn);
	if(CombatInterface != nullptr)
	{
		float duration = CombatInterface->PerformAttack(AttackType);
		UE_LOG(LogTemp,Log,TEXT("PerformAttack::ExecuteTask duration %f"),duration)
		GetWorld()->GetTimerManager().SetTimer(WaitFinishHandle,[this,&OwnerComp]()
		{
			UE_LOG(LogTemp,Log,TEXT("PerformAttack::ExecuteTask Timer"))
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		},0.1f,false,duration);	
	}
	
	return EBTNodeResult::InProgress;
}

void UPerformAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	UE_LOG(LogTemp,Log,TEXT("PerformAttack::OnTaskFinished"))
	if(WaitFinishHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitFinishHandle);
	}
}
