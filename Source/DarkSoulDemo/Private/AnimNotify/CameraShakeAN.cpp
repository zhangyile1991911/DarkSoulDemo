// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/CameraShakeAN.h"

#include "Kismet/GameplayStatics.h"

void UCameraShakeAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* OwnerActor = MeshComp->GetOwner();
	if(!IsValid(OwnerActor))
	{
		return;
	}
	FVector ActorLocation = OwnerActor->GetActorLocation();
	UGameplayStatics::PlayWorldCameraShake(OwnerActor,ShakeClass,ActorLocation,InnerRadius,OuterRadius,Falloff);
}
