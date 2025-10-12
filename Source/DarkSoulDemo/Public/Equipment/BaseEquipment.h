// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquipment.generated.h"

class APickupItem;

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickupItem> PickupItemClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D> EquipmentIcon;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual TObjectPtr<UStaticMesh> GetEquipmentMesh()const{return EquipmentMesh;}
	TObjectPtr<UStaticMeshComponent> GetStaticComponent()const{return StaticMesh;}
	virtual void EquipItem();
	virtual void UnequipItem();
	virtual void AttachToPlayer(FName SocketName);
	virtual void DetachFromPlayer(FName SocketName);

	virtual void GeneratePickupActor();
	void DisableMeshCollision();
};
