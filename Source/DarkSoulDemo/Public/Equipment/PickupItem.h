// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DarkSoulDemo/Public/Interface/Interact.h"
#include "PickupItem.generated.h"

class ABaseEquipment;

UCLASS()
class DARKSOULDEMO_API APickupItem : public AActor,public IInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupItem();

protected:
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseEquipment> BaseEquipment;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact(AActor* Actor) override;
};
