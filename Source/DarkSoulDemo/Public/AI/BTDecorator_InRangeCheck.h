// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "BTDecorator_InRangeCheck.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UBTDecorator_InRangeCheck : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float GreaterThan;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LessThan;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetKeySelector;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
