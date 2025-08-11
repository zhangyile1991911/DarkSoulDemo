// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStats.generated.h"


UENUM(BlueprintType)
enum class ECharaStats : UINT8
{
	Health UMETA(DisplayName="Health"),
	Stamina UMETA(DisplayName="Stamina")
};

DECLARE_MULTICAST_DELEGATE(FDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateStatEvent,ECharaStats,CharaStats,float,newVal);

const float PER_REGENERATE_STAMINA = 0.2f;
//管理 角色属性 HP 耐力
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULDEMO_API UCharacterStats : public UActorComponent
{
	GENERATED_BODY()

	FTimerHandle RegenerateStaminaHandle;
protected:
	//Health
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Property")
	float BaseHealth = 100.0f;
	float MaxHealth = 100.0f;

	//Stamina
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Property")
    float BaseStamina = 100.0f;
    float MaxStamina = 100.0f;
public:
	void Heal(float HealAmount);
	void TakeDamage(float DamageAmount);
private:
	float StaminaRegenRate = PER_REGENERATE_STAMINA;
public:
	bool DecreaseStamina(float StaminaCost);
	bool CheckHasEnoughStamina(float StaminaCost);
	void PauseRegenerateStamina(){StaminaRegenRate = 0.0f;}
	void ResumeRegenerateStamina(){StaminaRegenRate = PER_REGENERATE_STAMINA;}

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina()const{return MaxStamina;}
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth()const{return MaxHealth;}
	
	float GetBaseHealth()const{return BaseHealth;}
	float GetBaseStamina()const{return BaseStamina;}
	//Defence
private:
	float DefenceStat;
public:
	void IncreaseDefence(float DefenceAmount);
	void DecreaseDefence(float DefenceAmount);
	UFUNCTION(BlueprintCallable)
	void ResetStats();
	void DecreaseHealth(float value);
	bool IsDead()const{return BaseHealth <= 0;}
public:	
	// Sets default values for this component's properties
	UCharacterStats();

	FDeathEvent DeathEvent;

	UPROPERTY(BlueprintAssignable)
	FUpdateStatEvent StatChangedEvent;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void RegenerateStamina();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};