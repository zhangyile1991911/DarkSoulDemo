// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Components/ActorComponent.h"
#include "EMontageAction.h"
#include "CharacterState.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Attacking)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Attacking_LightAttack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Attacking_HeavyAttack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Attacking_SpecialAttack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Blocking)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Death)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Potion)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_GeneralAction)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Hit)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Parried)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Parrying)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Rolling)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Stunned)



class ADarkSoulDemoCharacter;
// 用于管理 玩家状态 移动 攻击
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UCharacterState : public UActorComponent
{
	GENERATED_BODY()
	
	
	UPROPERTY()
	TObjectPtr<ADarkSoulDemoCharacter> ParentActor;

	EMontageAction CurrentAttack;
	// FGameplayTag CurrentState;
	FGameplayTagContainer StateContainer;
protected:
	UPROPERTY(BlueprintReadOnly)
	bool MovementInput = true;
public:
	// Sets default values for this component's properties
	UCharacterState();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	// void SetState(FGameplayTag newState){CurrentState = newState;}
	void AddState(FGameplayTag newState);
	void RemoveState(FGameplayTag removeState);
	void ClearAllState();
	void RecoverToNormalState();
	const FGameplayTagContainer& GetGameplayContainer(){return StateContainer;}
	void DisableMovement(){MovementInput = false;}
	void EnableMovement(){MovementInput = true;}
	bool CanMovement()const{return MovementInput;}
	void SetAttackAction(EMontageAction newAction){CurrentAttack = newAction;}
	EMontageAction GetAttackAction()const{return CurrentAttack;}
};
