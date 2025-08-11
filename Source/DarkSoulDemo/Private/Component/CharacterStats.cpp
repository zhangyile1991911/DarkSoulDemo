// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulDemo/Public/Component/CharacterStats.h"

#include "MathUtil.h"

// Sets default values for this component's properties
UCharacterStats::UCharacterStats()
{
	PrimaryComponentTick.bCanEverTick = false;
	UE_LOG(LogInit,Display,TEXT("UCharacterStats::UCharacterStats()"));
}


// Called when the game starts
void UCharacterStats::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogInit,Display,TEXT("UCharacterStats::BeginPlay()"));
	
	GetWorld()->GetTimerManager().SetTimer(RegenerateStaminaHandle,this,&UCharacterStats::RegenerateStamina,0.1f,true);
}


void UCharacterStats::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogInit,Display,TEXT("UCharacterStats::BeginDestroy()"));
	
	
}

void UCharacterStats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(RegenerateStaminaHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerateStaminaHandle);	
	}
}


// Called every frame
void UCharacterStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCharacterStats::DecreaseStamina(float StaminaCost)
{
	if(!CheckHasEnoughStamina(StaminaCost))
	{
		return false;
	}
	BaseStamina = FMathf::Clamp(BaseStamina - StaminaCost,0,MaxStamina);
	if(StatChangedEvent.IsBound())
	{
		StatChangedEvent.Broadcast(ECharaStats::Stamina,BaseStamina);
	}
	
	return true;
}

void UCharacterStats::RegenerateStamina()
{
	BaseStamina += StaminaRegenRate;
	BaseStamina = FMathf::Clamp(BaseStamina,0,MaxStamina);
	
	if(StatChangedEvent.IsBound())
	{
		StatChangedEvent.Broadcast(ECharaStats::Stamina,BaseStamina);
	}
}

bool UCharacterStats::CheckHasEnoughStamina(float StaminaCost)
{
	return BaseStamina >= StaminaCost;
}

void UCharacterStats::TakeDamage(float DamageAmount)
{
	if(BaseHealth <= 0)
		return;
	BaseHealth -= DamageAmount;
	StatChangedEvent.Broadcast(ECharaStats::Health,BaseHealth);
	if(BaseHealth <=0)
	{
		DeathEvent.Broadcast();
	}
}

void UCharacterStats::IncreaseDefence(float DefenceAmount)
{
	DefenceStat += DefenceAmount;
}

void UCharacterStats::DecreaseDefence(float DefenceAmount)
{
	DefenceStat -= DefenceAmount;
	if(DefenceStat < 0)
		DefenceStat = 0;
}

void UCharacterStats::ResetStats()
{
	BaseHealth = MaxHealth;
	BaseStamina = MaxStamina;
	if(StatChangedEvent.IsBound())
	{
		StatChangedEvent.Broadcast(ECharaStats::Stamina,BaseStamina);
		StatChangedEvent.Broadcast(ECharaStats::Health,BaseHealth);
	}
}

void UCharacterStats::DecreaseHealth(float value)
{
	BaseHealth -= value;
	if(BaseHealth <= 0.0f && DeathEvent.IsBound())
	{
		DeathEvent.Broadcast();
	}
}

void UCharacterStats::Heal(float HealAmount)
{
	BaseHealth += HealAmount;
	BaseHealth = FMathf::Clamp(BaseHealth,0,MaxHealth);
	StatChangedEvent.Broadcast(ECharaStats::Health,BaseHealth);
}
