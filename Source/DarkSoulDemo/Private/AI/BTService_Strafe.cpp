// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Strafe.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
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
	// ANavigationData* data = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	// FVector origin = ControlledPawn->GetActorLocation();
	// FNavLocation randomLocation;
	// //获取随机点
	// bool bsuccess = NavSys->GetRandomPointInNavigableRadius(origin,Radius,randomLocation,data,nullptr);
	// if(bsuccess)
	// {
	// 	OwnerController->MoveToLocation(randomLocation);	
	// }
	UBlackboardComponent* BlackboardComponent = OwnerController->GetBlackboardComponent();
	if(!IsValid(BlackboardComponent))
	{
		return;
	}

	//拿到target
	UObject* obj = BlackboardComponent->GetValueAsObject(FName("Target"));
	ACharacter* TargetCharacter = Cast<ACharacter>(obj);
	if(!IsValid(TargetCharacter))
	{
		return;
	}
	//计算地面接触点 
	ACharacter* Character = Cast<ACharacter>(ControlledPawn);
	FVector origin = Character->GetActorLocation();
	origin.Z -= Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FVector targetLocation = TargetCharacter->GetActorLocation();
	targetLocation.Z -= TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FVector directionToTarget = (targetLocation - origin).GetSafeNormal();
	const float distToTarget = FVector::Distance(targetLocation,origin);
	FVector directionToMove = (origin.RightVector + directionToTarget) * MoveDistance;
	if(distToTarget < KeepDistance)
	{
		UE_LOG(LogTemp,Log,TEXT("Player is too Closet %f DeltaSeconds %f"),distToTarget,DeltaSeconds)
		directionToMove = (origin.LeftVector + directionToTarget ) * (-MoveDistance);
	}
	
	
	FVector dest = directionToMove + origin;
	DrawDebugBox(GetWorld(),
		dest,
		FVector(20,20,20),
		FQuat::Identity,
		FColor::Blue,
		false,
		5.0f,
		0,
		2.0f);
	
	// FPathFindingQuery Query;
	// Query.StartLocation = origin;
	// Query.EndLocation = dest;
	// Query.NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	// Query.Owner = OwnerController;
	//
	// FPathFindingResult Result = NavSys->FindPathSync(Query);
	//
	// if (Result.IsSuccessful())
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("FindPathSync 可以从 Start 到达 Target"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("FindPathSync 无法到达目标点"));
	// }
	
	//
	FNavLocation ProjectedLocation;
	bool bOnNav = NavSys->ProjectPointToNavigation(dest,ProjectedLocation);
	if (bOnNav)
	{
		UE_LOG(LogTemp, Log, TEXT("ProjectPointToNavigation 可以从 Start 到达 Target"));
		OwnerController->MoveToLocation(dest);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectPointToNavigation 无法到达目标点"));
		//获取随机点
		FNavLocation randomLocation;
	    ANavigationData* data = NavSys->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
		bool bsuccess = NavSys->GetRandomPointInNavigableRadius(origin,Radius,randomLocation,data,nullptr);
		if(bsuccess)
		{
			OwnerController->MoveToLocation(randomLocation);	
		}
	}
	
	
	
	// if(bOnNav)
	// {
	// 	UE_LOG(LogTemp,Log,TEXT("Player is too Closet bOnNav True"))
	// 	OwnerController->MoveToLocation(ProjectedLocation);
	// }
	// else
	// {
		//获取随机点
	// 	FNavLocation randomLocation;
	//     auto data = NavSys->GetDefaultNavDataInstance(FNavigationSystem::ECreateIfEmpty::DontCreate);
	// 	bool bsuccess = NavSys->GetRandomPointInNavigableRadius(origin,Radius,randomLocation,data,nullptr);
	// 	if(bsuccess)
	// 	{
	// 		OwnerController->MoveToLocation(randomLocation);	
	// 	}
	// DrawDebugBox(GetWorld(),
	// 	randomLocation,
	// 	FVector(20,20,20),
	// 	FQuat::Identity,
	// 	FColor::Blue,
	// 	false,
	// 	5.0f,
	// 	0,
	// 	2.0f);
	// }
	
}
