// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UFindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector BlackboardLocationKey;
};
