// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DarkSoulDemo/Public/Interface/Interact.h"
#include "Campfire.generated.h"

UCLASS()
class DARKSOULDEMO_API ACampfire : public AActor,public IInteract
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
public:
	// Sets default values for this actor's properties
	ACampfire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Actor) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCampfire(AActor* Actor);
};
