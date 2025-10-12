// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UEnemyManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<AActor*> Enemies;
public:
	UEnemyManagerSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void RegisterEnemy(AActor* Actor);
	void UnregisterEnemy(AActor* Actor);

	const TArray<AActor*>& GetAllEnemies(){return Enemies;}
};
