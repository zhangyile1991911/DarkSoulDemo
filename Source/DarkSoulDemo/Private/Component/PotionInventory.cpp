// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PotionInventory.h"

#include "Component/CharacterStats.h"
#include "DarkSoulDemo/DarkSoulDemoCharacter.h"


// Sets default values for this component's properties
UPotionInventory::UPotionInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPotionInventory::BeginPlay()
{
	Super::BeginPlay();
	PotionQuantity = 5;
	// ...
	
}


// Called every frame
void UPotionInventory::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPotionInventory::RefreshPotionNum()
{
	if(PotionChangeEvent.IsBound())
		PotionChangeEvent.Broadcast(PotionQuantity);
}

void UPotionInventory::DrinkPotion()
{
	if(PotionQuantity <= 0)return;
	AActor* owner = GetOwner();
	UCharacterStats* CharacterStats = owner->GetComponentByClass<UCharacterStats>();
	CharacterStats->Heal(PotionHealAmount);

	PotionQuantity--;
	
	if(PotionChangeEvent.IsBound())
		PotionChangeEvent.Broadcast(PotionQuantity);
}

void UPotionInventory::AddPotionQuantity(int num)
{
	PotionQuantity += FMath::Clamp(num,0,num);
	if(PotionChangeEvent.IsBound())
		PotionChangeEvent.Broadcast(PotionQuantity);
}

