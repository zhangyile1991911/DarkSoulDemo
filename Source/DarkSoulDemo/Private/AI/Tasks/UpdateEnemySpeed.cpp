// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/UpdateEnemySpeed.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UUpdateEnemySpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if(!IsValid(OwnerController))
	{
		return EBTNodeResult::Succeeded;
	}

	APawn* ControlledPawn = OwnerController->GetPawn();
	if(!IsValid(ControlledPawn))
	{
		return EBTNodeResult::Succeeded;
	}
	
	UCharacterMovementComponent* MovementComponent = ControlledPawn->GetComponentByClass<UCharacterMovementComponent>();
	MovementComponent->MaxWalkSpeed = MaxWalkSpeed;
	
	return EBTNodeResult::Succeeded;
}
