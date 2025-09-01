// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotionInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UPotionInventory : public UActorComponent
{
	GENERATED_BODY()
protected:
	
	int PotionQuantity;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PotionHealAmount;
public:
	// Sets default values for this component's properties
	UPotionInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	void DrinkPotion();
};
