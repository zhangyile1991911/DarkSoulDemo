// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Strafe.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UBTService_Strafe : public UBTService
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;
	UPROPERTY()
	TObjectPtr<AAIController> OwnerController;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float KeepDistance = 10.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MoveDistance = 100.0f;
public:
	UBTService_Strafe();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
