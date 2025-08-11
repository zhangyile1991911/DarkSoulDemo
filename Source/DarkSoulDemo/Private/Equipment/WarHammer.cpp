// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/WarHammer.h"

#include "Component/WeaponCollision.h"


// Sets default values
AWarHammer::AWarHammer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseCollision->SocketStart = "WeaponStart";
	BaseCollision->SocketEnd = "WeaponEnd";
	BaseCollision->SphereRadius = 10.0f;

	HandleCollision = CreateDefaultSubobject<UWeaponCollision>(TEXT("WarHammerHandle"),true);
	HandleCollision->SetWeapon(this->StaticMesh);
	HandleCollision->SocketStart = "WeaponHandleStart";
	HandleCollision->SocketEnd = "WeaponHandleEnd";
	HandleCollision->SphereRadius = 5.0f;

	CombatType = ECombatType::TwoHand;
}

// Called when the game starts or when spawned
void AWarHammer::BeginPlay()
{
	Super::BeginPlay();
	DelegateOfHandle = HandleCollision->DamageActorDelegate.AddUObject(this,&ThisClass::HandleDamageActor);	
}

void AWarHammer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	HandleCollision->DamageActorDelegate.Remove(DelegateOfHandle);
	
}

// Called every frame
void AWarHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarHammer::ActivateCollision(EWeaponCollisionType param)
{
	BaseCollision->TurnOnCollision();
	HandleCollision->TurnOnCollision();
}

void AWarHammer::DeactiveCollision(EWeaponCollisionType param)
{
	BaseCollision->TurnOffCollision();
	HandleCollision->TurnOffCollision();
}

void AWarHammer::EquipItem()
{
	Super::EquipItem();
	HandleCollision->SetWeapon(StaticMesh);
	HandleCollision->AddIgnoreActor(GetOwner());
}

void AWarHammer::UnequipItem()
{
	Super::UnequipItem();
	HandleCollision->SetWeapon(nullptr);
}

