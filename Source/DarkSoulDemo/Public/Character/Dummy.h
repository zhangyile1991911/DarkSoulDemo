// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CanTargeting.h"
#include "Dummy.generated.h"

UCLASS()
class DARKSOULDEMO_API ADummy : public ACharacter,public ICanTargeting
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float  Damage, class AController*  InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType*  DamageType, AActor* DamageCauser);

	virtual bool CanBeTargetd()override;
	virtual void OnTargeted(bool bIsTargeted)override;
};
