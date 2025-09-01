// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ParryAN.h"

#include "Component/CharacterState.h"

void UParryAN::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* Owner = MeshComp->GetOwner();
	if(!IsValid(Owner))return;
	
	UCharacterState* stateComp = Owner->GetComponentByClass<UCharacterState>();
	if(!IsValid(stateComp))return;

	stateComp->AddState(Player_State_Parrying);
}

void UParryAN::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AActor* Owner = MeshComp->GetOwner();
    if(!IsValid(Owner))return;
    
    UCharacterState* stateComp = Owner->GetComponentByClass<UCharacterState>();
    if(!IsValid(stateComp))return;

    stateComp->RemoveState(Player_State_Parrying);
}
