// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/KnockBackAN.h"

#include "Character/BaseEnemy.h"
#include "Component/CharacterCombat.h"
#include "Equipment/BaseWeapon.h"
#include "Kismet/GameplayStatics.h"

void UKnockBackAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FVector SocketLocation = MeshComp->GetSocketLocation(LocationSocket);
	TArray<AActor*> IgnoreActors;
	AActor* Owner = MeshComp->GetOwner();
	do
	{
		if(!IsValid(Owner))break;
		//todo 加上其他己方队友
		IgnoreActors.Add(Owner);

		APawn* Pawn = Cast<APawn>(Owner);
		if(!IsValid(Pawn))break;

		AController* Controller = Pawn->GetController();
		if(!IsValid(Controller))break;

		UCharacterCombat* CombatComp = Pawn->GetComponentByClass<UCharacterCombat>();
		if(!IsValid(CombatComp))break;

		const float BaseDamage = CombatComp->GetMainWeapon()->MakeDamage(EMontageAction::SpecialAttack);
		
		UGameplayStatics::ApplyRadialDamage(
			Owner,
			BaseDamage,
			SocketLocation,
			DamageRadius,
			DamageTypeClass,
			IgnoreActors,
			Owner,
			Controller);
		
	}
	while (false);
	
	DrawDebugSphere(Owner->GetWorld(),SocketLocation,DamageRadius,16,FColor::Black,false,10,0,3);
}
