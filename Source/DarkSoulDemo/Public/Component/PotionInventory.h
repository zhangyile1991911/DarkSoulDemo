// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotionInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePotionEvent,int,PotionNum);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UPotionInventory : public UActorComponent
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly)
	int PotionQuantity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PotionHealAmount;

	UPROPERTY(BlueprintAssignable)
	FUpdatePotionEvent PotionChangeEvent;
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

	void RefreshPotionNum();
	void DrinkPotion();
	bool HasPotion()const{return PotionQuantity > 0;}
	UFUNCTION(BlueprintCallable)
	void AddPotionQuantity(int num);
};
