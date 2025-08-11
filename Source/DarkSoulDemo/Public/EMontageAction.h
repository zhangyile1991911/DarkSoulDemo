#pragma once

UENUM(BlueprintType)
enum class EMontageAction : uint8
{
	None UMETA(Displayname="None"),
	Equip UMETA(DisplayName="Equip"),
	Unequip UMETA(DisplayName="Unequip"),
	LightAttack UMETA(DisplayName="LightAttack"),
	HeavyAttack UMETA(DisplayName="HeavyAttack"),
	AirAttack UMETA(DisplayName="AirAttack"),
	RunningAttack UMETA(DisplayName="RunningAttack"),
	SpecialAttack UMETA(DisplayName="SpecialAttack"),
	HitReaction UMETA(DisplayName="HitReaction"),
	DirectionalRolling UMETA(DisplayName="DirectionalRolling"),
};