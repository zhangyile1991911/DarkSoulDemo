// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Fists.generated.h"

UCLASS(Blueprintable)
class DARKSOULDEMO_API AFists : public ABaseWeapon
{
	GENERATED_BODY()
	
	FDelegateHandle RightHandHandle;
	FDelegateHandle LeftHandHandle;
protected:
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite)
	TObjectPtr<UWeaponCollision> LeftHand;
public:
	// Sets default values for this actor's properties
	AFists();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ActivateCollision(EWeaponCollisionType param) override;
	virtual void DeactiveCollision(EWeaponCollisionType param) override;
	void EquipItem() override;
	void UnequipItem() override;
};
