// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ECombatType.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Engine/EngineTypes.h"
#include "UI/MVVMModel/PlayerStatsModel.h"
#include "Interface/CombatInterface.h"
#include "DarkSoulDemoCharacter.generated.h"

class UTargeting;
enum class EMontageAction : uint8;
class ABaseWeapon;
class UCharacterStats;
class UCharacterState;
class UCharacterCombat;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPotionInventory;
class UEnhancedInputLocalPlayerSubsystem;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADarkSoulDemoCharacter : public ACharacter,public ICombatInterface
{
	GENERATED_BODY()

public:
	//Begin CombatInterface
	virtual void ActivateCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactiveCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual float PerformAttack(EMontageAction AttackType) override;
	virtual void Parried(AActor* Actor) override;
	//End CombatInterface
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShiftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConsumeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultWeapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> DefaultWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AnimMontage",meta=(AllowPrivateAccess="true"))
	UAnimMontage* DrinkPotion;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AnimMontage",meta=(AllowPrivateAccess="true"))
	UAnimMontage* KnockBack;
	
	UPROPERTY(EditAnywhere,Category= Sound)
	USoundBase* HitSound;
	
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	TArray<AActor*> TargetCandidate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> PotionBP;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* PlayerInputSubSystem;
	// int TargetCandidateIndex;
public:
	ADarkSoulDemoCharacter();
	

protected:
	//处理输入
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Rolling(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);

	// void OnGoing(const FInputActionValue& Value)
	// {
	// 	UE_LOG(LogTemp,Log,TEXT("RollAction OnGoing"));
	// }
	void OnSprintCompleted(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void ToggleWeapon(const FInputActionValue& Value);

	void ToggleBlocking(const FInputActionValue& Value);
	void ReleaseBlocking(const FInputActionValue& Value);

	void ToggleParry(const FInputActionValue& Value);

	void PerformanceAttack(const FInputActionValue& Value);

	void PerformanceHeavyAttack(const FInputActionValue& Value);

	void PerformanceSpecialAttack(const FInputActionValue& Value);

	void ToggleTarget(const FInputActionValue& Value);

	void SwitchTargetLeft(const FInputActionValue& Value);
	void SwitchTargetRight(const FInputActionValue& Value);

	void TogglePotion(const FInputActionValue& Value);
protected:
	void ToggleWeaponInner();
	bool CanPerformanceAttack();
	bool CanAutomaticallyDrawWeapon();
	void StartDelayTime(float latent,FTimerDelegate::TMethodPtr<ADarkSoulDemoCharacter> delayCallback);
	void StartDelayTime(float latent,TFunction<void()>&& delayCallback);
	void ClearStateAndRegenerateStamina(FGameplayTag removeTag);
	void EquipDefaultWeapon();
	void HandleAttack(EMontageAction,FGameplayTag);
	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float  Damage, class AController*  InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType*  DamageType, AActor* DamageCauser);
	UFUNCTION()
	void TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser);
	void HandleDeathEvent();
	UFUNCTION()
	void WatchMontagedEnd(UAnimMontage* Montage,bool bInterrupted);
	bool CanBlockAttack(const AActor* InstigatorActor);
	bool IsFacingActor(const AActor* InstigatorActor);
	void TakeHitBack(AController* InstigatedBy,float Damage);
	void TakeKnockBack(AController* InstigatedBy,float Damage);
	void DoCameraShake();
protected:
	//Montage
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* RollingAnimMontage;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TargetedRollingAnimMontage;
	// UPROPERTY(BlueprintReadOnly)
	// UPlayerStatsModel* PlayerStatsViewModel;
protected:
	//组件
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterCombat> CombatComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterState> StateComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterStats> StatsComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UTargeting> TargetingComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UPotionInventory> PotionComponent;
protected:
	//状态
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	ECombatType CurrentCombatType;

	UPROPERTY()
	TObjectPtr<AActor> PotionMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraShake")
	float InnerRadius;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraShake")
	float OuterRadius;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraShake")
	float Falloff;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual ECombatType CombatType()override{return CurrentCombatType;}

	// UFUNCTION(BlueprintCallable)
	// UPlayerStatsModel* GetPlayerStatsViewModel();

	UFUNCTION(BlueprintCallable)
	void RefreshCharacterStats();
	
	// virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};