// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EHitDirection.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DarkSoulSystemLibrary.generated.h"


/**
 * 
 */
UCLASS()
class DARKSOULDEMO_API UDarkSoulSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,Category="DarkSoulSystem|Caculator",meta=(DefaultToSelf="WorldContextObject"))
	static float CalculateZAxisRotation(FVector a,FVector b,bool isRad = false);

	UFUNCTION(BlueprintPure,Category="DarkSoulSystem|Caculator",meta=(DefaultToSelf="WorldContextObject"))
	static EHitDirection ConvertToHitDirection(float Degress);
};
