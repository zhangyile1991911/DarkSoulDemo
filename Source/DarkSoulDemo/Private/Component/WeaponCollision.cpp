// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulDemo/Public/Component/WeaponCollision.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UWeaponCollision::UWeaponCollision()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponCollision::BeginPlay()
{
	Super::BeginPlay();

	// TraceObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	UE_LOG(LogTemp,Log,TEXT("WeaponCollision Owner Name = %s"),*GetOwner()->GetName())
	IgnoreActors.Add(GetOwner());
	if(TraceObjectTypes.IsEmpty())
	{
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	}
	// ...
}

void UWeaponCollision::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UWeaponCollision::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bCollisionOn)
	{
		CollisionTrace();
	}
}

void UWeaponCollision::AddIgnoreActor(AActor* Actor)
{
	if(IsValid(Actor) && IgnoreActors.Find(Actor) == INDEX_NONE)
	{
		IgnoreActors.Add(Actor);
	}
}

void UWeaponCollision::ClearAlreadyHit()
{
	AlreadyHit.Empty();
}

void UWeaponCollision::TurnOnCollision()
{
	ClearAlreadyHit();
	bCollisionOn = true;
}

void UWeaponCollision::TurnOffCollision()
{
	ClearAlreadyHit();
	bCollisionOn = false;
}


void UWeaponCollision::CollisionTrace()
{
	if(!IsValid(Weapon))return;
	// UE_LOG(LogTemp,Log,TEXT("%s TickComponent"),*this->GetName())
	// UE_LOG(LogTemp, Warning, TEXT("%s CollisionTrace SocketStart: %s"),*this->GetName(), *SocketStart.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s CollisionTrace SocketEnd: %s"), *this->GetName(),*SocketEnd.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s CollisionTrace SphereRadius: %f"),*this->GetName(), SphereRadius);
	FVector WorldStartPosition = Weapon->GetSocketLocation(SocketStart);
	FVector WorldEndPosition = Weapon->GetSocketLocation(SocketEnd);
	
	TArray<FHitResult> results;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		WorldStartPosition,
		WorldEndPosition,
		SphereRadius,
		TraceObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		results,
		true);
	
	if(!bHit)return;
	
	for(const FHitResult& one : results)
	{
		AActor* actor = one.GetActor();
		if(AlreadyHit.Find(actor))
		{
			continue;
		}
		AlreadyHit.Add(actor);
		//广播事件,通知给父Actor
		DamageActorDelegate.Broadcast(one);
	}
}

