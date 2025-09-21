// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Strafe.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UNavigationSystemV1;

UBTService_Strafe::UBTService_Strafe()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTService_Strafe::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	OwnerController = OwnerComp.GetAIOwner();
	if(!IsValid(OwnerController))return;

	ControlledPawn = OwnerController->GetPawn();
	if(!IsValid(ControlledPawn))
	{
		return;	
	}

	UBlackboardComponent* BlackboardComponent = OwnerController->GetBlackboardComponent();
	if(!IsValid(BlackboardComponent))return;

	UObject* obj = BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName);
	AActor* targetActor = Cast<AActor>(obj);
	if(!IsValid(targetActor))return;
	//设置目标
	OwnerController->SetFocus(targetActor);

	const ACharacter* Character = Cast<ACharacter>(ControlledPawn);
	if(IsValid(Character))
	{
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;	
	}
}

void UBTService_Strafe::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	//清除目标
	OwnerController->ClearFocus(EAIFocusPriority::Default);
	
	const ACharacter* Character = Cast<ACharacter>(ControlledPawn);
	if(IsValid(Character))
	{
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void UBTService_Strafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//1 获取当前世界
	UWorld* CurWorld = GEngine->GetWorldFromContextObject(ControlledPawn,EGetWorldErrorMode::LogAndReturnNull);
	if(CurWorld == nullptr)return;
	//2 拿到当前世界导航信息
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(CurWorld);
	if(!IsValid(NavSys))return;
	//3. 获取导航数据
	ANavigationData* data = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	FVector origin = ControlledPawn->GetActorLocation();
	FNavLocation randomLocation;
	//获取随机点
	bool bsuccess = NavSys->GetRandomPointInNavigableRadius(origin,Radius,randomLocation,data,nullptr);
	if(bsuccess)
	{
		OwnerController->MoveToLocation(randomLocation);	
	}
}
