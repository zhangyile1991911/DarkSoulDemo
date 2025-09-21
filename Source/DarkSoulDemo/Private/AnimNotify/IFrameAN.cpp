// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/IFrameAN.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UIFrameAN::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* OwnerActor = MeshComp->GetOwner();
	if(!IsValid(OwnerActor))return;

	ACharacter* Character = Cast<ACharacter>(OwnerActor);
	if(!IsValid(Character))return;
	
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if(!IsValid(CapsuleComponent))return;
	// 虽然可以通过设置CollisionResponse开启无敌帧 但是 会穿过Boss
	OriginCollisionResponse = CapsuleComponent->GetCollisionResponseToChannel(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}

void UIFrameAN::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if(!IsValid(OwnerActor))return;

	ACharacter* Character = Cast<ACharacter>(OwnerActor);
	if(!IsValid(Character))return;
	
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if(!IsValid(CapsuleComponent))return;

	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,OriginCollisionResponse);
}
