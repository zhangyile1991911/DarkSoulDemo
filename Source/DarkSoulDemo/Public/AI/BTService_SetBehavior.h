// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetBehavior.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DARKSOULDEMO_API UBTService_SetBehavior : public UBTService
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	UPROPERTY()
	TObjectPtr<ACharacter> AIPawn;
	
protected:
	//Begin UBTService
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//End UBTService
	
	//UBTService -> BTAuxiliaryNode-> UBTNode(这个基类中提供的方法)
	virtual void SetOwner(AActor* ActorOwner) override;

	void UpdateBehavior();
public:
	UBTService_SetBehavior();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackRangeDistance;
};