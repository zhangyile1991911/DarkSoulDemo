// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DarkSoulAIController.generated.h"

UCLASS()
class DARKSOULDEMO_API ADarkSoulAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	float invertal = 0.1f;
public:
	// Sets default values for this actor's properties
	ADarkSoulAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void UpdateTarget();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
