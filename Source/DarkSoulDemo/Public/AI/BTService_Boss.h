// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Boss.generated.h"

class ABaseEnemy;
/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UBTService_Boss : public UBTService
{
	GENERATED_BODY()

	TObjectPtr<AAIController> AIController;
	TObjectPtr<ABaseEnemy> BaseEnemy;
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void SetOwner(AActor* ActorOwner) override;

	void UpdateBehavior();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackRangeDistance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BossStaminaCheck;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector BehaviourKey;
public:
	UBTService_Boss();
};
