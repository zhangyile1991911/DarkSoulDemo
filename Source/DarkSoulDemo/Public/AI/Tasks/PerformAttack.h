// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMontageAction.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PerformAttack.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UPerformAttack : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EMontageAction AttackType;

	UPROPERTY()
	FTimerHandle WaitFinishHandle;
public:
	
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
