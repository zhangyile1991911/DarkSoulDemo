// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DarkSoulDamageType.generated.h"

enum class EDamageType : uint8;
/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UDarkSoulDamageType : public UDamageType
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EDamageType DamageType;
public:
	EDamageType GetDamageType()const{return DamageType;}
};
