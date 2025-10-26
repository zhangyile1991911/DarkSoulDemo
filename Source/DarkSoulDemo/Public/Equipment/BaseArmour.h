// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipment.h"
#include "SkeletalRenderPublic.h"
#include "BaseArmour.generated.h"

UCLASS(Blueprintable)
class DARKSOULDEMO_API ABaseArmour : public ABaseEquipment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
public:
	// Sets default values for this actor's properties
	ABaseArmour(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void AttachToPlayer(FName SocketName) override;
	virtual void DetachFromPlayer(FName SocketName) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
};