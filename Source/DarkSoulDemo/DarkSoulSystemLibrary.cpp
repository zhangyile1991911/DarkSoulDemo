// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulSystemLibrary.h"

float UDarkSoulSystemLibrary::CalculateZAxisRotation(FVector a, FVector b,bool isRad)
{
	if(!a.IsNormalized())a.Normalize();
	if(!b.IsNormalized())b.Normalize();
	
	FVector AXY(a.X,a.Y,0);
	FVector BXY(b.X,b.Y,0);

	AXY.Normalize();
	BXY.Normalize();
	float dot = FVector::DotProduct(AXY,BXY);
	float AngleRad = FMath::Acos(dot);
	if(isRad)
	{
		return AngleRad;
	}
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float CrossZ = AXY.X * BXY.Y - AXY.Y * BXY.X;
	if(CrossZ < 0)
	{
		AngleDeg = -AngleDeg;
	}
	return AngleDeg;
}

EHitDirection UDarkSoulSystemLibrary::ConvertToHitDirection(float Degress)
{
	if(Degress <= 45.f && Degress >= -45.0f)
	{
		return EHitDirection::FRONT;
	}
	if(Degress >= 45.f && Degress <= 45.0f + 90.0f )
	{
		return EHitDirection::LEFT;
	}
	if(Degress <= -45.0f && Degress >= -135.0f)
	{
		return EHitDirection::RIGHT;
	}
	return EHitDirection::BACK;
}
