// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Targeting.generated.h"


class UCameraComponent;
class ACharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UTargeting : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCameraComponent> OwnerCamera;

	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsLockOn = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TargetRotationSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TargetingRadius = 500.0f;
public:
	// Sets default values for this component's properties
	UTargeting();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	TArray<AActor*> FindLockOnTargets();
	AActor* FindClosetTarget(const TArray<AActor*>& Targets);

	void OrientCamera();
	void OrientMovement();

	void StartLockOnTarget(AActor* Target);
	void StopLockOnTarget();
	bool isLockOnTarget()const{return bIsLockOn;}
	TObjectPtr<AActor> GetCurrentLockOnTarget()const{return LockOnTarget;}
};
