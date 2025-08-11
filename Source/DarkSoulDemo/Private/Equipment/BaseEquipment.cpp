// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/BaseEquipment.h"

#include "GameFramework/Character.h"


// Sets default values
ABaseEquipment::ABaseEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEquipment::EquipItem()
{
}

void ABaseEquipment::UnequipItem()
{
}

void ABaseEquipment::AttachToPlayer(FName SocketName)
{
	//Cast用于UObject的类型转换
	//static_cast 用于传统cpp类型转换
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	FAttachmentTransformRules rules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
		);
	AttachToComponent(Character->GetMesh(),rules);
	
}

// Called every frame
void ABaseEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

