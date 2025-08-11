// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulDemo/Public/Component/CharacterState.h"


UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking,"Player.State.Attacking")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking_LightAttack,"Player.State.Attacking.LightAttack")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking_HeavyAttack,"Player.State.Attacking.HeavyAttack")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking_SpecialAttack,"Player.State.Attacking.SpecialAttack")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Blocking,"Player.State.Blocking")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Death,"Player.State.Death")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Potion,"Player.State.DrinkingPotion")
UE_DEFINE_GAMEPLAY_TAG(Player_State_GeneralAction,"Player.State.GeneralAction")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Hit,"Player.State.Hit")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Parried,"Player.State.Parried")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Parrying,"Player.State.Parrying")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Rolling,"Player.State.Rolling")
UE_DEFINE_GAMEPLAY_TAG(Player_State_Stunned,"Player.State.Stunned")

// Sets default values for this component's properties
UCharacterState::UCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterState::AddState(FGameplayTag newState)
{
	StateContainer.AddTag(newState);
}

void UCharacterState::RemoveState(FGameplayTag removeState)
{
	StateContainer.RemoveTag(removeState);
}

void UCharacterState::ClearAllState()
{
	StateContainer.Reset(0);
}

void UCharacterState::RecoverToNormalState()
{
	ClearAllState();
	StateContainer.AddTag(Player_State_GeneralAction);
}

