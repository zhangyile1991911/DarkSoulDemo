// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/Fists.h"

#include "Component/CharacterCombat.h"
#include "Component/WeaponCollision.h"
#include "GameFramework/Character.h"
#include "Interface/SyncMsgToAnim.h"


// Sets default values
AFists::AFists()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BaseCollision->SocketEnd = FName("hand_r");
	BaseCollision->SocketStart = FName("hand_r");
	BaseCollision->SphereRadius = 5.0f;
	
	LeftHand = CreateDefaultSubobject<UWeaponCollision>(TEXT("LeftHand"),true);
	LeftHand->SocketStart = FName("hand_l");
	LeftHand->SocketEnd = FName("hand_l");
	LeftHand->SphereRadius = 5.0f;

	CombatType = ECombatType::Fists;
}

// Called when the game starts or when spawned
void AFists::BeginPlay()
{
	Super::BeginPlay();
	LeftHandHandle = LeftHand->DamageActorDelegate.AddUObject(this,&ThisClass::HandleDamageActor);
}

void AFists::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LeftHand->DamageActorDelegate.RemoveAll(this);
}

// Called every frame
void AFists::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFists::ActivateCollision(EWeaponCollisionType param)
{
	if(param == EWeaponCollisionType::RightHand)
	{
		BaseCollision->TurnOnCollision();	
	}
	if(param == EWeaponCollisionType::LeftHand)
	{
		LeftHand->TurnOnCollision();	
	}
}

void AFists::DeactiveCollision(EWeaponCollisionType param)
{
	if(param == EWeaponCollisionType::RightHand)
	{
		BaseCollision->TurnOffCollision();	
	}
	if(param == EWeaponCollisionType::LeftHand)
	{
		LeftHand->TurnOffCollision();	
	}
}

void AFists::EquipItem()
{
	// Super::EquipItem();
	ACharacter* Character = Cast<ACharacter>(Owner);
	if(IsValid(Owner) && IsValid(Character))
	{

		UCharacterCombat* CombatComponent = GetOwner()->GetComponentByClass<UCharacterCombat>();
		CombatComponent->SetWeapon(this);
		CombatComponent->EnableCombat();
		
		BaseCollision->AddIgnoreActor(Character);
		BaseCollision->SetWeapon(Character->GetMesh());

		LeftHand->AddIgnoreActor(Character);
		LeftHand->SetWeapon(Character->GetMesh());
		
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
	
}

void AFists::UnequipItem()
{
	Super::UnequipItem();
	BaseCollision->SetWeapon(nullptr);
	LeftHand->SetWeapon(nullptr);
	UCharacterCombat* combat = GetOwner()->GetComponentByClass<UCharacterCombat>();
	combat->DisableCombat();
}

