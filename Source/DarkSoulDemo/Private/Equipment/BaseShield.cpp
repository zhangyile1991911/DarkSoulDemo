// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/BaseShield.h"

#include "GameFramework/Character.h"


// Sets default values
ABaseShield::ABaseShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseShield::EquipItem()
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,true);
	ACharacter* character = Cast<ACharacter>(GetOwner());
	this->AttachToComponent(character->GetMesh(),Rules,EquippedSocket);
}

void ABaseShield::UnequipItem()
{
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,true);
	ACharacter* character = Cast<ACharacter>(GetOwner());
	this->AttachToComponent(character->GetMesh(),Rules,UnEquippedSocket);
}

