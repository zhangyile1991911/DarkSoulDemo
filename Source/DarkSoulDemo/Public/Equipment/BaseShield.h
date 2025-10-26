// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipment.h"
#include "BaseShield.generated.h"

UCLASS()
class DARKSOULDEMO_API ABaseShield : public ABaseEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseShield(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName EquippedSocket;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FName UnEquippedSocket;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
};
