// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCombat.generated.h"


class ABaseShield;
class ABaseWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentChanged,EEquipmentType,EquipmentType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UCharacterCombat : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCharacterCombat();
protected:
	UPROPERTY(BlueprintReadOnly,Transient)
	bool bCombatEnable = true;
	
	UPROPERTY(BlueprintReadOnly,Transient)
	bool bBlockEnable = false;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

	UPROPERTY()
	TObjectPtr<ABaseShield> MainShield;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeapon(TObjectPtr<ABaseWeapon> InWeapon);
	void SetShield(TObjectPtr<ABaseShield> InShield);

	UFUNCTION(BlueprintCallable)
	ABaseWeapon* GetMainWeapon()const{return MainWeapon;}

	UFUNCTION(BlueprintCallable)
	ABaseShield* GetMainShield()const{return MainShield;}
	
	bool GetBlockEnable()const{return bBlockEnable;}
	bool CanEnableCombat()const{return bCombatEnable;}
	void EnableCombat(){bCombatEnable = true;}
	void DisableCombat(){bCombatEnable = false;}
	void EnableBlock(){bBlockEnable = true;}
	void DisableBlock(){bBlockEnable = false;}

	UFUNCTION(BlueprintCallable)
	bool HasMainShield()const{return MainShield != nullptr;}

	void DisableCollision();

	UPROPERTY(BlueprintAssignable)
	FEquipmentChanged OnEquipmentChanged;
};
