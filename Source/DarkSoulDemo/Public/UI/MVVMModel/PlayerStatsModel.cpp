// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVMModel/PlayerStatsModel.h"

#include "MathUtil.h"
#include "Component/CharacterStats.h"

void UPlayerStatsModel::BeginDestroy()
{
	Super::BeginDestroy();
	StatsComponent = nullptr;
}

void UPlayerStatsModel::InitializeWithStatsComponent(UCharacterStats* InStatsComponent)
{
	if(!IsValid(InStatsComponent))
	{
		UE_LOG(LogTemp,Error,TEXT("UPlayerStatsModel::InitializeWithStatsComponent InStatsComponent is null"))
		return;
	}
	StatsComponent = InStatsComponent;
	StatsComponent->StatChangedEvent.AddDynamic(this,&ThisClass::ListenStatsChanged);
}

void UPlayerStatsModel::SetStaminaPercent(float InStamina)
{
	float max = StatsComponent->GetMaxStamina();
	float base = StatsComponent->GetBaseStamina();
	float percent = FMathf::Clamp(base/max,0,1.0f);
	UE_MVVM_SET_PROPERTY_VALUE(StaminaPercent,percent);
}

void UPlayerStatsModel::SetHealthPercent(float InHealthPoint)
{
	float max = StatsComponent->GetMaxHealth();
	float base = StatsComponent->GetBaseHealth();
	float percent = FMathf::Clamp(base/max,0,1.0f);
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent,percent);
}

void UPlayerStatsModel::ListenStatsChanged(ECharaStats CharaStats,float newVal)
{
	switch (CharaStats)
	{
	case ECharaStats::Health:
		SetHealthPercent(newVal);
		break;
	case ECharaStats::Stamina:
		SetStaminaPercent(newVal);
		break;
	}
}
