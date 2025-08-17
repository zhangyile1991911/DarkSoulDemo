// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterRotator.h"


// Sets default values for this component's properties
UCharacterRotator::UCharacterRotator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterRotator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterRotator::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!ShouldRotate)return;
	
	if(!IsValid(Target))return;
	
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector DirectionOwnerToTarget = TargetLocation - OwnerLocation;
	FRotator X = FRotationMatrix::MakeFromX(DirectionOwnerToTarget).Rotator();
	GetOwner()->SetActorRotation(FRotator(0,X.Yaw,0));
}

