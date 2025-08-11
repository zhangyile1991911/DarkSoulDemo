// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/JadeSword.h"

#include "Component/WeaponCollision.h"
#include "Equipment/BaseWeapon.h"

// Sets default values
AJadeSword::AJadeSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseCollision->SocketStart = "WeaponStart";
	BaseCollision->SocketEnd = "WeaponEnd";
	BaseCollision->SphereRadius = 5.0f;
	CombatType = ECombatType::SingleSword;
}

// Called when the game starts or when spawned
void AJadeSword::BeginPlay()
{
	Super::BeginPlay();
	ResetCombo();
}

// Called every frame
void AJadeSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
