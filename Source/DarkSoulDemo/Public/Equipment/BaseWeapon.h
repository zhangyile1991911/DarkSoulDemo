// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipment.h"
#include "EMontageAction.h"
#include "GameFramework/Actor.h"
#include "ECombatType.h"
#include "EWeaponCollisionType.h"
#include "BaseWeapon.generated.h"


class UWeaponCollision;

UCLASS(Blueprintable)
class DARKSOULDEMO_API ABaseWeapon : public ABaseEquipment
{
	GENERATED_BODY()

	FDelegateHandle DamageActorHandle;
protected:
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
    ECombatType CombatType = ECombatType::None;
public:
	// Sets default values for this actor's properties
	ABaseWeapon();
	ECombatType GetCombatType()const{return CombatType;}
protected:
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite)
	TObjectPtr<UWeaponCollision> BaseCollision;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* AnimationDataTable;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName EquippedSocket;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName UnEquippedSocket;

	UPROPERTY(BlueprintReadOnly)
	int comboIndex = 0;

	static constexpr float MaxInputInterval = 2.5f;
	
	UPROPERTY(BlueprintReadOnly)
	float nextInputInterval = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float baseDamage = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float HeavyAttackMultiplier = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float AirAttackMultiplier = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float RunningAttackMultiplier = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float ChargedAttackMultiplier = 0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float SpecialAttackMultiplier = 0.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	virtual void HandleDamageActor(const FHitResult& hitData);
	
	void AdvanceStep(TObjectPtr<UAnimMontage> AnimMontagePtr);
	void CheckStepContinuity(float DeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EquipItem() override;
	void UnequipItem() override;
	void AttachToPlayer(FName SocketName)override;
	const FName& GetEquippedSocket()const{return EquippedSocket;}
	const FName& GetUnEquippedSocket()const{return UnEquippedSocket;}

	void ResetCombo(){comboIndex = 0;}
	int CurCombo()const{return comboIndex;}
	float GetCostStamina(EMontageAction)const;

	virtual void ActivateCollision(EWeaponCollisionType param);
	virtual void DeactiveCollision(EWeaponCollisionType param);
	
	virtual TTuple<TObjectPtr<UAnimMontage>,FName> GetMontageForAction(EMontageAction Action);
	
	virtual float MakeDamage(EMontageAction Action);
};