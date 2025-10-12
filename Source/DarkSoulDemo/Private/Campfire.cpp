// Fill out your copyright notice in the Description page of Project Settings.


#include "Campfire.h"

#include "DarkSoulDemo/DarkSoulDemoCharacter.h"
#include "GameFramework/Character.h"


// Sets default values
ACampfire::ACampfire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACampfire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACampfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACampfire::Interact(AActor* Actor)
{
	if(!IsValid(Actor))return;

	ADarkSoulDemoCharacter* Character = Cast<ADarkSoulDemoCharacter>(Actor);
	if(!IsValid(Character))return;
	//播放动作
	Character->PlayCampfireSave();
	//播放粒子特效
	UpdateCampfire(Character);
	
}

