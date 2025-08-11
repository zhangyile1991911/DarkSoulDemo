// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCollision.generated.h"

/*
 * 原生的cppActorComponent中的成员变量
 * 直接放到blueprint中无法通过EditDefaultsOnly设置变量
 * 必须继承一个blueprint
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FDamageActorSignature,const FHitResult&)
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKSOULDEMO_API UWeaponCollision : public UActorComponent
{
	GENERATED_BODY()

	bool bCollisionOn = false;
	void CollisionTrace();
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> Weapon;
	
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	UPROPERTY()
	FHitResult LastHitResult;

	TSet<AActor*> AlreadyHit;
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
public:
	// Sets default values for this component's properties
	UWeaponCollision();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName SocketStart;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName SocketEnd;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SphereRadius;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void AddIgnoreActor(AActor* Actor);
	void ClearAlreadyHit();
	void TurnOnCollision();
	void TurnOffCollision();
	void SetWeapon(TObjectPtr<UPrimitiveComponent> InWeapon){Weapon = InWeapon;}

	FDamageActorSignature DamageActorDelegate;
};
