// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCombat.generated.h"


class ABaseWeapon;

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
	bool bBlockEnable;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetWeapon(TObjectPtr<ABaseWeapon> InWeapon);
	TObjectPtr<ABaseWeapon> GetMainWeapon()const{return MainWeapon;}
	bool GetBlockEnable()const{return bBlockEnable;}
	bool CanEnableCombat()const{return bCombatEnable;}
	void EnableCombat(){bCombatEnable = true;}
	void DisableCombat(){bCombatEnable = false;}
};
