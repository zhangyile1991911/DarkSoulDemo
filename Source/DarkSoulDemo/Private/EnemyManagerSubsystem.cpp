// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManagerSubsystem.h"

UEnemyManagerSubsystem::UEnemyManagerSubsystem()
{
	Enemies.Reserve(8);
}

void UEnemyManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp,Log,TEXT("UEnemyManagerSubsystem::Initialize"))
}

void UEnemyManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp,Log,TEXT("UEnemyManagerSubsystem::Deinitialize"))
	//WorldSubsystem会在Actor的BeginDestroy之前调用
	//所以WorldSubsystem只是保持引用，并不会参与管理Actor的生命周期
	Enemies.Empty();
}

void UEnemyManagerSubsystem::RegisterEnemy(AActor* Actor)
{
	if(!IsValid(Actor))return;
	UE_LOG(LogTemp,Log,TEXT("UEnemyManagerSubsystem RegisterEnemy %s"),*Actor->GetName())
	Enemies.AddUnique(Actor);
}

void UEnemyManagerSubsystem::UnregisterEnemy(AActor* Actor)
{
	if(!IsValid(Actor))return;
	UE_LOG(LogTemp,Log,TEXT("UEnemyManagerSubsystem UnregisterEnemy %s"),*Actor->GetName())
	Enemies.Remove(Actor);
}
