// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/PickupItem.h"

#include "AsyncTreeDifferences.h"
#include "Component/CharacterCombat.h"
#include "Equipment/BaseArmour.h"
#include "Equipment/BaseEquipment.h"
#include "Equipment/BaseShield.h"
#include "Equipment/BaseWeapon.h"


// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void APickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UE_LOG(LogTemp,Log,TEXT("APickupItem::OnConstruction"))
	if(IsValid(BaseEquipment))
	{
		TObjectPtr<UStaticMesh> mesh = BaseEquipment.GetDefaultObject()->GetEquipmentMesh();
		if(mesh != nullptr)
		{
			StaticMesh->SetStaticMesh(mesh);
			//开启物理
			StaticMesh->SetSimulatePhysics(true);
		}
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
	UCharacterCombat* CharacterCombat = Actor->GetComponentByClass<UCharacterCombat>();
	if(!IsValid(CharacterCombat))return;
	
	//生成捡起的武器 
	const FTransform& ActorTransform = Actor->GetActorTransform();
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	params.Owner = Actor;
	TObjectPtr<AActor> NewActor = GetWorld()->SpawnActor(BaseEquipment,&ActorTransform,params);
	TObjectPtr<ABaseEquipment> NewEquipment = Cast<ABaseEquipment>(NewActor);
	//判断当前携带的武器
	// TObjectPtr<ABaseWeapon> MainWeapon = CharacterCombat->GetMainWeapon();
	do
	{
		bool isWeapon = NewEquipment->GetClass()->IsChildOf<ABaseWeapon>();
		if(isWeapon)
		{
			ABaseWeapon* weapon = Cast<ABaseWeapon>(NewEquipment);
			CharacterCombat->SetWeapon(weapon);
			if(CharacterCombat->CanEnableCombat())
			{
				NewEquipment->EquipItem();	
			}
			else
			{
				NewEquipment->UnequipItem();	
			}
			
			break;
		}
		
		bool isShield = NewEquipment->GetClass()->IsChildOf<ABaseShield>();
		if(isShield)
		{
			ABaseShield* shield = Cast<ABaseShield>(NewEquipment);
			CharacterCombat->SetShield(shield);
			ABaseWeapon* weapon = CharacterCombat->GetMainWeapon();
			if(weapon == nullptr)break;
			if(weapon->GetCombatType() != ECombatType::SingleSword)
			{
				NewEquipment->UnequipItem();
				break;
			}
			if(CharacterCombat->CanEnableCombat())
			{
				NewEquipment->EquipItem();
			}
			else
			{
				NewEquipment->UnequipItem();
			}
			break;
		}
		bool isArmor = NewEquipment->GetClass()->IsChildOf<ABaseArmour>();
		if(isArmor)
		{
			NewEquipment->EquipItem();
		}
	}
	while (false);
	
	GetWorld()->DestroyActor(this);
}

