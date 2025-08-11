// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatBar.h"

void UStatBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	UE_LOG(LogInit,Display,TEXT("UStatBar::NativePreConstruct()"));
}

void UStatBar::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogInit,Display,TEXT("UStatBar::NativeConstruct()"));
}

void UStatBar::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogExit,Display,TEXT("UStatBar::BeginDestroy()"));
}
