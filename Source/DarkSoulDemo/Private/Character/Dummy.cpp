// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dummy.h"


// Sets default values
ADummy::ADummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();
	OnTakePointDamage.AddDynamic(this,&ThisClass::TakePointDamage);
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADummy::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
	UE_LOG(LogTemp,Log,TEXT("TakePointDamageActor %s DamageActor %s Damage %f"),
		*this->GetActorNameOrLabel(),
		*DamagedActor->GetName(),
		Damage);
}

bool ADummy::CanBeTargetd()
{
	return false;
}

void ADummy::OnTargeted(bool bIsTargeted)
{
}

