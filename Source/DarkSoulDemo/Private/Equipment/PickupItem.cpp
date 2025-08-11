// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/PickupItem.h"

#include "Equipment/BaseEquipment.h"


// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UE_LOG(LogTemp,Log,TEXT("APickupItem::OnConstruction"))
	if(IsValid(BaseEquipment))
	{
		TObjectPtr<UStaticMesh> mesh = BaseEquipment.GetDefaultObject()->GetEquipmentMesh();
		StaticMesh->SetStaticMesh(mesh);
		//开启物理
		StaticMesh->SetSimulatePhysics(true);
	}
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupItem::Interact(AActor* Actor)
{
	UE_LOG(LogTemp,Log,TEXT("APickupItem::Interact"))
	//捡起武器 生成
	const FTransform& ActorTransform = Actor->GetActorTransform();
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	TObjectPtr<AActor> NewActor = GetWorld()->SpawnActor(BaseEquipment,&ActorTransform,params);
	NewActor->SetOwner(Actor);
	TObjectPtr<ABaseEquipment> NewEquipment = Cast<ABaseEquipment>(NewActor);
	NewEquipment->EquipItem();

	GetWorld()->DestroyActor(this);
}

