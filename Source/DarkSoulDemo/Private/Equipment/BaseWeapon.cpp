// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulDemo/Public/Equipment/BaseWeapon.h"

#include "MathUtil.h"
#include "Component/CharacterCombat.h"
#include "Component/CharacterState.h"
#include "Component/CharacterStats.h"
#include "Component/WeaponCollision.h"
#include "DarkSoulDemo/DarkSoulDemoCharacter.h"
#include "DataTableDefine/FMontageAction.h"
#include "GameFramework/Character.h"
#include "Interface/SyncMsgToAnim.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseCollision = CreateDefaultSubobject<UWeaponCollision>(TEXT("Collision"),true);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	DamageActorHandle = BaseCollision->DamageActorDelegate.AddUObject(this,&ThisClass::HandleDamageActor);
}

void ABaseWeapon::BeginDestroy()
{
	Super::BeginDestroy();
	BaseCollision->DamageActorDelegate.Remove(DamageActorHandle);
}



// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckStepContinuity(DeltaTime);
}

void ABaseWeapon::HandleDamageActor(const FHitResult& hitData)
{
	if(hitData.GetActor() == nullptr)return;

	AActor* DamageActor = hitData.GetActor();

	UCharacterState* stateComp = Owner->GetComponentByClass<UCharacterState>();
	if(!IsValid(stateComp)) return;
	
	float BaseDamage = MakeDamage(stateComp->GetAttackAction());
	FVector forward = Owner->GetActorForwardVector();
	UActorComponent* ActorComponent = DamageActor->GetComponentByClass(UCharacterStats::StaticClass());

	UCharacterStats* CharacterStats = Cast<UCharacterStats>(ActorComponent);
	if(!IsValid(ActorComponent))
	{
		return;	
	}
	
	if(!IsValid(CharacterStats) || CharacterStats->IsDead()) return;
	
	UGameplayStatics::ApplyPointDamage(DamageActor,
			BaseDamage,
			forward,
			hitData,
			Owner->GetInstigatorController(),
			this,
			nullptr);
	
}

void ABaseWeapon::EquipItem()
{
	UCharacterCombat* combat = GetOwner()->GetComponentByClass<UCharacterCombat>();
	if(!IsValid(combat))
	{
		UE_LOG(LogTemp,Log,TEXT("ABaseWeapon::EquipItem UCharacterCombat == null"))
	}
	combat->SetWeapon(this);
	combat->EnableCombat();
	
	FName SocketName = combat->CanEnableCombat() ? EquippedSocket : UnEquippedSocket;
	AttachToPlayer(SocketName);
	BaseCollision->SetWeapon(StaticMesh);
	BaseCollision->AddIgnoreActor(GetOwner());
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance))
	{
		bool bImplemented = AnimInstance->Implements<USyncMsgToAnim>();
		if(bImplemented)
		{
			ISyncMsgToAnim::Execute_SyncCombatMode(AnimInstance,CombatType);
		}
	}
	
}

void ABaseWeapon::UnequipItem()
{
	UCharacterCombat* combat = GetOwner()->GetComponentByClass<UCharacterCombat>();
	combat->DisableCombat();
	AttachToPlayer(UnEquippedSocket);
	BaseCollision->SetWeapon(nullptr);
}

void ABaseWeapon::AttachToPlayer(FName SocketName)
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true);
	
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	AttachToComponent(Character->GetMesh(),Rules,SocketName);
}

float ABaseWeapon::GetCostStamina(EMontageAction Action) const
{
	FString EnumName = StaticEnum<EMontageAction>()->GetNameStringByValue(static_cast<int64>(Action));
	FName ActionName(EnumName);
	
	FMontageAction* row = AnimationDataTable->FindRow<FMontageAction>(ActionName,"");
	if(row == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("ABaseWeapon::GetCostStamina Row = null %d"),Action)
		return 5.0f;
	}
	return row->CostStamina;
}

void ABaseWeapon::ActivateCollision(EWeaponCollisionType param)
{
	BaseCollision->TurnOnCollision();
}

void ABaseWeapon::DeactiveCollision(EWeaponCollisionType param)
{
	BaseCollision->TurnOffCollision();
}

TTuple<TObjectPtr<UAnimMontage>,FName> ABaseWeapon::GetMontageForAction(EMontageAction Action)
{
	TTuple<TObjectPtr<UAnimMontage>,FName> Result;
	
	FString EnumName = StaticEnum<EMontageAction>()->GetNameStringByValue(static_cast<int64>(Action));
	FName ActionName(EnumName);

	FMontageAction* row = AnimationDataTable->FindRow<FMontageAction>(ActionName,"");
	if(row == nullptr)
	{
		UE_LOG(LogTemp,Log,TEXT("ABaseWeapon::GetMontageForAction Row = null %d"),Action)
		Result.Key = nullptr;
		Result.Value = "";
	}
	else
	{
		Result.Key = row->Animation;
		Result.Value = row->Animation->GetSectionName(comboIndex);
		AdvanceStep(row->Animation);
	}
	
	return MoveTemp(Result);
}

float ABaseWeapon::MakeDamage(EMontageAction Action)
{
	float damage = 0;
	switch(Action)
	{
		case EMontageAction::LightAttack:
			damage = baseDamage;
			break;
		case EMontageAction::AirAttack:
			damage = baseDamage * AirAttackMultiplier;
			break;
		case EMontageAction::HeavyAttack:
			damage = baseDamage * HeavyAttackMultiplier;
			break;
		case EMontageAction::RunningAttack:
			damage = baseDamage * RunningAttackMultiplier;
			break;
		case EMontageAction::SpecialAttack:
			damage = baseDamage * SpecialAttackMultiplier;
			break;
		default:
			damage = baseDamage;
	}
	return damage;
}

void ABaseWeapon::AdvanceStep(TObjectPtr<UAnimMontage> AnimMontagePtr)
{
	if(AnimMontagePtr == nullptr)
	{
		ResetCombo();
		return;
	}
	const int lastIndex = AnimMontagePtr->GetNumSections() - 1;
	comboIndex += 1;
	if(comboIndex > lastIndex)
	{
		ResetCombo();
	}
	nextInputInterval = MaxInputInterval;
}

void ABaseWeapon::CheckStepContinuity(float DeltaTime)
{
	nextInputInterval -= DeltaTime;
	nextInputInterval = FMathf::Clamp(nextInputInterval,0,MaxInputInterval);
	if(nextInputInterval <= 0)
	{
		ResetCombo();
	}
}
