// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CanTargeting.h"
#include "Interface/CombatInterface.h"
#include "BaseEnemy.generated.h"

class ATargetPoint;
class UWidgetComponent;
class UBehaviorTree;
class UCharacterStats;
class UCharacterState;
class UCharacterCombat;
class ABaseWeapon;

UCLASS()
class DARKSOULDEMO_API ABaseEnemy : public ACharacter,public ICanTargeting,public ICombatInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultWeapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> DefaultWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviourTree;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterState> StateComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterStats> StatsComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterCombat> CombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> LockOnWidget;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> HPBarWidget;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(BlueprintReadOnly,Category="Patrol")
	int PatrolIndex;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ShowWeaponMeshEditor;//这个mesh只是为了在Editor中设置默认武器时候展示用,运行时候会删除
public:
	// Sets default values for this character's properties
	ABaseEnemy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void ListenDeathEvent();
	UFUNCTION()
	void HandlePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UBehaviorTree* GetBehaviorTree()const{return BehaviourTree;}
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanBeTargetd()override;
	
	virtual void OnTargeted(bool bIsTargeted)override;

	UFUNCTION(BlueprintCallable)
	ATargetPoint* GetCurrentPatrolPoint();

	//ICombatInterface Begin

	virtual void ActivateCollision(EWeaponCollisionType WeaponCollisionType)override;
	virtual void DeactiveCollision(EWeaponCollisionType WeaponCollisionType)override;
	virtual float PerformAttack(EMontageAction AttackType) override;
	
	//ICombatInterface End
	
	void EquipDefaultWeapon();
};

