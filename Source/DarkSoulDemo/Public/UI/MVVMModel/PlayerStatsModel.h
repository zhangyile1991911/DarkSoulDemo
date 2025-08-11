// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "PlayerStatsModel.generated.h"

class UCharacterStats;
/**
 * 
 */
UCLASS(Blueprintable)
class DARKSOULDEMO_API UPlayerStatsModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void BeginDestroy() override;
	
	UFUNCTION(BlueprintCallable)
	void InitializeWithStatsComponent(UCharacterStats* InStatsComponent);

	//对应UPROPERTY中设置的Getter和Setter 必须提供对应函数
	float GetStaminaPercent()const{return StaminaPercent;}
	void SetStaminaPercent(float InStamina);
	
	float GetHealthPercent()const{return HealthPercent;}
	void SetHealthPercent(float InHealthPoint);
private:
	UPROPERTY()
	TObjectPtr<UCharacterStats> StatsComponent;

	UFUNCTION()
	void ListenStatsChanged(ECharaStats CharaStats,float newVal);
protected:
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess,Bindable))
	float StaminaPercent;

	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess,Bindable))
	float HealthPercent;
};
