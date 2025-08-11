// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterCombat.h"

#include "Equipment/BaseWeapon.h"
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


// Called every frame
void UCharacterCombat::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterCombat::SetWeapon(TObjectPtr<ABaseWeapon> InWeapon)
{
	if(MainWeapon && MainWeapon->GetClass() != AFists::StaticClass())
	{
		//先删除现在手里的武器
		const FTransform& ActorTransform = GetOwner()->GetActorTransform();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
		params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		GetWorld()->SpawnActor(MainWeapon->GetClass(),&ActorTransform,params);
		GetWorld()->DestroyActor(MainWeapon);
		MainWeapon = nullptr;
	}
	if(InWeapon)
	{
		MainWeapon = InWeapon;
	}
	else
	{
		MainWeapon = nullptr;
	}
}

