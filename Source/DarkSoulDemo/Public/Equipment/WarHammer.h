// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WarHammer.generated.h"

UCLASS(Blueprintable)
class DARKSOULDEMO_API AWarHammer : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite)
	TObjectPtr<UWeaponCollision> HandleCollision;

	FDelegateHandle DelegateOfHandle;
public:
	// Sets default values for this actor's properties
	AWarHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ActivateCollision(EWeaponCollisionType param) override;
	void DeactiveCollision(EWeaponCollisionType param) override;
	void EquipItem() override;
	void UnequipItem() override;
};
