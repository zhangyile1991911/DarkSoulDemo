// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterCombat.h"

#include "EEquipmentType.h"
#include "EnemyManagerSubsystem.h"
#include "Equipment/BaseWeapon.h"
#include "Equipment/BaseShield.h"
#include "Equipment/Fists.h"


// Sets default values for this component's properties
UCharacterCombat::UCharacterCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterCombat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCharacterCombat::BeginDestroy()
{
	Super::BeginDestroy();
}


// Called every frame
void UCharacterCombat::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterCombat::SetWeapon(TObjectPtr<ABaseWeapon> InWeapon)
{
	if(IsValid(MainWeapon))
	{
		bool bIsFist = MainWeapon->IsA<AFists>();
		if(!bIsFist)
		{
			MainWeapon->GeneratePickupActor();
			//先删除现在手里的武器
			// const FTransform& ActorTransform = GetOwner()->GetActorTransform();
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
			params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			// GetWorld()->SpawnActor(MainWeapon->GetClass(),&ActorTransform,params);
			GetWorld()->DestroyActor(MainWeapon);
			MainWeapon = nullptr;
		}
		bool isTwoHand = InWeapon->GetCombatType() == ECombatType::TwoHand;
		bool hasMainShield = MainShield != nullptr;
		if(isTwoHand && hasMainShield)
		{
			MainShield->UnequipItem();
		}
	}
	
	if(InWeapon)
	{
		MainWeapon = InWeapon;
	}
	else
	{
		MainWeapon = nullptr;
	}
	if(OnEquipmentChanged.IsBound())
	{
		OnEquipmentChanged.Broadcast(EEquipmentType::Weapon);
	}
}

void UCharacterCombat::SetShield(TObjectPtr<ABaseShield> InShield)
{
	if(IsValid(MainShield))
	{
		const FTransform& ActorTransform = GetOwner()->GetActorTransform();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
		params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		GetWorld()->SpawnActor(MainWeapon->GetClass(),&ActorTransform,params);
		GetWorld()->DestroyActor(MainWeapon);
		MainWeapon = nullptr;	
	}
	if(InShield)
	{
		MainShield = InShield;
	}
	else
	{
		MainShield = nullptr;
	}
	if(OnEquipmentChanged.IsBound())
	{
		OnEquipmentChanged.Broadcast(EEquipmentType::Shield);
	}
}

void UCharacterCombat::DisableCollision()
{
	if(IsValid(MainWeapon))
	{
		MainWeapon->DisableMeshCollision();
	}
	if(IsValid(MainShield))
	{
		MainShield->DisableMeshCollision();
	}
}

