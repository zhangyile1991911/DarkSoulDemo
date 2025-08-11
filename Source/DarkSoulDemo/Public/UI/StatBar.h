// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseMVVMWidget.h"
#include "StatBar.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DARKSOULDEMO_API UStatBar : public UBaseMVVMWidget
{
	GENERATED_BODY()
protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void BeginDestroy() override;
};
