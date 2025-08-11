// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquipment.generated.h"

UCLASS(Blueprintable)
class DARKSOULDEMO_API ABaseEquipment : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseEquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> EquipmentMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TObjectPtr<UStaticMesh> GetEquipmentMesh()const{return EquipmentMesh;}

	virtual void EquipItem();
	virtual void UnequipItem();
	virtual void AttachToPlayer(FName SocketName);
	
};
