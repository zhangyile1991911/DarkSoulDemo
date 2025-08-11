// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Targeting.h"

#include "Camera/CameraComponent.h"
#include "Interface/CanTargeting.h"
#include "Kismet/GameplayStatics.h"
#include "../DarkSoulTraceChannels.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTargeting::UTargeting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargeting::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* owner = GetOwner();
	OwnerCharacter = Cast<ACharacter>(owner);
}


// Called every frame
void UTargeting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bIsLockOn)return;

	FRotator controllerRotator = OwnerCharacter->GetController()->GetControlRotation();

	FVector targetLocation = LockOnTarget->GetActorLocation();
	targetLocation.Z -= 150.f;
	FRotator needToRotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),targetLocation);
	FRotator interp = FMath::RInterpTo(controllerRotator,needToRotator,DeltaTime,TargetRotationSpeed);
	interp.Roll = controllerRotator.Roll;
	OwnerCharacter->GetController()->SetControlRotation(interp);
	
}

TArray<AActor*> UTargeting::FindLockOnTargets()
{
	TArray<FHitResult> HitResults;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Targeting));
	
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());
	
	FVector ActorLocation = GetOwner()->GetActorLocation();
	//1 获得范围内的Actor
	// UKismetSystemLibrary::SphereTraceMulti(
	// 	this,
	// 	ActorLocation,
	// 	ActorLocation,
	// 	TargetingRadius,
	// 	TraceTypeQuery11,false,IgnoreActor,EDrawDebugTrace::ForDuration,HitResults,true);
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this,
		ActorLocation,
		ActorLocation,
		TargetingRadius,
		ObjectTypes,
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true);
	//2 是否继承BeTargeting接口
	TArray<AActor*> TargetArr;
	if(isHit)
	{
		
		for (auto one : HitResults)
		{
			AActor* HitActor = one.GetActor();
			//Cast：
			// 内部调用 UObject::GetInterfaceAddress，需遍历接口表并计算偏移量。
			// 即使失败，也会执行完整的查找流程。
			// ICanTargeting* implementInterface = Cast<ICanTargeting>(HitActor);
			// if(implementInterface != nullptr)
			// {
			// 	TargetArr.Add(HitActor);
			// }
			// ImplementsInterface：
			// 直接调用 UClass::ImplementsInterface，仅检查接口表是否存在目标接口。
			// 无指针计算，纯粹的逻辑判断。
			bool isImplement = HitActor->GetClass()->ImplementsInterface(UCanTargeting::StaticClass());
			if(isImplement)
			{
				TargetArr.Add(HitActor);
			}
		}
	}
	return MoveTemp(TargetArr);
}

AActor* UTargeting::FindClosetTarget(const TArray<AActor*>& Targets)
{
	FHitResult result;

	AActor* ClosetActor = nullptr;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());

	float MaxDistance = FLT_MAX;
	OwnerCamera = GetOwner()->GetComponentByClass<UCameraComponent>();
	//1 通过相机发射射线判断哪个最近
	FVector CameraWorldPosition = OwnerCamera->GetComponentTransform().GetLocation();
	for(int i = 0; i < Targets.Num();i++)
	{
		const AActor* Actor = Targets[i];
		FVector ActorWorldPosition = Actor->GetTransform().GetLocation();
		//这里发射线目的是为了确认 目标是否可以被相机看到
		bool isHit = UKismetSystemLibrary::LineTraceSingle(
			this,
			CameraWorldPosition,
			ActorWorldPosition,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			IgnoreActor,
			EDrawDebugTrace::ForDuration,
			result,
			true);
		if(!isHit)
		{
			continue;
		}
		float distance = FVector::Distance(ActorWorldPosition,GetOwner()->GetActorLocation());
		if(distance < MaxDistance)
		{
			ClosetActor = result.GetActor();
			MaxDistance = distance;
		}
	}
	
	return ClosetActor;
}

void UTargeting::OrientCamera()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UTargeting::OrientMovement()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UTargeting::StartLockOnTarget(AActor* Target)
{
	if(!IsValid(Target))return;

	LockOnTarget = Target;
	OrientCamera();
	bIsLockOn = true;
	
}

void UTargeting::StopLockOnTarget()
{
	OrientMovement();
	LockOnTarget = nullptr;
	bIsLockOn = false;
}

