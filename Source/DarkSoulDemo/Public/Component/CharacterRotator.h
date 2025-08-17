// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterRotator.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UCharacterRotator : public UActorComponent
{
	GENERATED_BODY()

	bool ShouldRotate = false;
	
	UPROPERTY()
	TObjectPtr<AActor> Target = nullptr;
public:
	// Sets default values for this component's properties
	UCharacterRotator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void ToggleRotation(bool on){ShouldRotate = on;}
	void UpdateTarget(AActor* newTarget){Target = newTarget;}
};
