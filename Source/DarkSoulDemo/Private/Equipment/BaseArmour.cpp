// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/BaseArmour.h"

#include "Component/CharacterStats.h"
#include "GameFramework/Character.h"


// Sets default values
ABaseArmour::ABaseArmour(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this,TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseArmour::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseArmour::AttachToPlayer(FName SocketName)
{
	USkeletalMesh* mesh = SkeletalMeshComponent->GetSkeletalMeshAsset();
	if(!IsValid(mesh))return;
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!IsValid(Character))return;

	UCharacterStats* CharacterStats = Character->GetComponentByClass<UCharacterStats>();
	if(!IsValid(CharacterStats))return;
	
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true);
	SkeletalMeshComponent->AttachToComponent(Character->GetMesh(),AttachRules);
	SkeletalMeshComponent->SetLeaderPoseComponent(Character->GetMesh(),false,false);
	CharacterStats->IncreaseDefence(10);

	StaticMesh->SetStaticMesh(nullptr);
	
}

void ABaseArmour::DetachFromPlayer(FName SocketName)
{
	SkeletalMeshComponent->SetLeaderPoseComponent(nullptr);

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		false);
	SkeletalMeshComponent->DetachFromComponent(DetachRules);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!IsValid(Character))return;

	UCharacterStats* CharacterStats = Character->GetComponentByClass<UCharacterStats>();
	if(!IsValid(CharacterStats))return;

	CharacterStats->DecreaseDefence(10);
	
	Destroy();
}

// Called every frame
void ABaseArmour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseArmour::EquipItem()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(IsValid(Character))
	{
		AttachToPlayer(FName());
		UCharacterStats* stats = Character->GetComponentByClass<UCharacterStats>();
		if(IsValid(stats))
		{
		
		}
	}
	
}

void ABaseArmour::UnequipItem()
{
	Super::UnequipItem();
	
}

