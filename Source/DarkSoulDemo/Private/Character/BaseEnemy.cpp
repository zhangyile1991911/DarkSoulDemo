// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseEnemy.h"

#include "BrainComponent.h"
#include "Component/CharacterState.h"
#include "Component/CharacterStats.h"
#include "Component/CharacterCombat.h"
#include "Component/CharacterRotator.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Controller/EnemyAIController.h"
#include "Equipment/BaseWeapon.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StateComponent = CreateDefaultSubobject<UCharacterState>(TEXT("State"));
	StatsComponent = CreateDefaultSubobject<UCharacterStats>(TEXT("Stats"));
	CombatComponent = CreateDefaultSubobject<UCharacterCombat>(TEXT("Combat"));
	RotatorComponent = CreateDefaultSubobject<UCharacterRotator>(TEXT("Rotator"));
	
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(RootComponent);

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(RootComponent);

	ShowWeaponMeshEditor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShowWeaponMeshEditor"));
	ShowWeaponMeshEditor->SetupAttachment(RootComponent);

	
	
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(ShowWeaponMeshEditor))
	{
		// ShowWeaponMeshEditor->UnregisterComponent();
		// ShowWeaponMeshEditor->DestroyComponent();
		// ShowWeaponMeshEditor = nullptr;
		ShowWeaponMeshEditor->SetStaticMesh(nullptr);
	}
	
	StatsComponent->DeathEvent.AddUObject(this,&ThisClass::ListenDeathEvent);
	OnTakePointDamage.AddDynamic(this,&ThisClass::HandlePointDamage);
	if(DefaultWeapon)
	{
		EquipDefaultWeapon();
	}
	
}

void ABaseEnemy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// if(DefaultWeapon)
	// {
	// 	EquipDefaultWeapon();
	// }
	// if(IsValid(DefaultWeapon))
	// {
	// 	TObjectPtr<UStaticMesh> mesh = DefaultWeapon.GetDefaultObject()->GetEquipmentMesh();
	// 	const FName& equipSocket = DefaultWeapon.GetDefaultObject()->GetEquippedSocket();
	// 	if(IsValid(ShowWeaponMeshEditor))
	// 	{
	// 		ShowWeaponMeshEditor->SetStaticMesh(mesh);
	// 		ShowWeaponMeshEditor->SetupAttachment(RootComponent,equipSocket);
	// 	}
	// }
}

void ABaseEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(LogTemp,Log,TEXT("ABaseEnemy::OnConstruction"))
	if(IsValid(DefaultWeapon))
	{
		TObjectPtr<UStaticMesh> mesh = DefaultWeapon.GetDefaultObject()->GetEquipmentMesh();
		const FName& equipSocket = DefaultWeapon.GetDefaultObject()->GetEquippedSocket();
		if(IsValid(ShowWeaponMeshEditor))
		{
			ShowWeaponMeshEditor->SetStaticMesh(mesh);
			FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true);
			ShowWeaponMeshEditor->AttachToComponent(GetMesh(),rules,equipSocket);
			// ShowWeaponMeshEditor->SetupAttachment(GetMesh(),equipSocket);
			// ShowWeaponMeshEditor->SetRelativeLocation(FVector::ZeroVector);
			// ShowWeaponMeshEditor->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void ABaseEnemy::ListenDeathEvent()
{
	StateComponent->AddState(Player_State_Death);
	//忽略Pawn的碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	//开启布娃娃系统模拟物理
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());
	if(IsValid(controller))
	{
		controller->GetBrainComponent()->StopLogic(FString("Enemy is Dead"));
	}
}

void ABaseEnemy::HandlePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	StatsComponent->TakeDamage(Damage);
	//通知PerceptionAI
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(this);
	if (PerceptionSystem)
	{//记录犯错 InInstigator应该传入对应的Pawn，但之前传入了InstigatedBy是个Controller
		FAIDamageEvent Event(DamagedActor, InstigatedBy->GetPawn(), Damage, HitLocation, HitLocation, FName());
		PerceptionSystem->OnEvent(Event);
	}
}


// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ABaseEnemy::CanBeTargetd()
{
	return !StatsComponent->IsDead();
}

void ABaseEnemy::OnTargeted(bool bIsTargeted)
{
	LockOnWidget->SetVisibility(bIsTargeted);
}

ATargetPoint* ABaseEnemy::GetCurrentPatrolPoint()
{
	int oldIndex = PatrolIndex;
	PatrolIndex += 1;
	if(PatrolIndex >= PatrolPoints.Num())
	{
		PatrolIndex = 0;
	}
	
	return PatrolPoints[oldIndex];
}

void ABaseEnemy::ActivateCollision(EWeaponCollisionType WeaponCollisionType)
{
	CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
}

void ABaseEnemy::DeactiveCollision(EWeaponCollisionType WeaponCollisionType)
{
	CombatComponent->GetMainWeapon()->DeactiveCollision(WeaponCollisionType);
}

float ABaseEnemy::PerformAttack(EMontageAction AttackType)
{
	TObjectPtr<ABaseWeapon> MainWeapon = CombatComponent->GetMainWeapon();
	TTuple<TObjectPtr<UAnimMontage>,FName> AnimMontage = MainWeapon->GetMontageForAction(AttackType);
	if(!IsValid(AnimMontage.Key)) return 0;
	int sectionIndex = AnimMontage.Key->GetSectionIndex(AnimMontage.Value);
	float duration = AnimMontage.Key->GetSectionLength(sectionIndex);
	PlayAnimMontage(AnimMontage.Key,1.0,AnimMontage.Value);
	
	return duration;
}

void ABaseEnemy::EquipDefaultWeapon()
{
	if(!DefaultWeapon)return;

	const FTransform& ActorTransform = GetActorTransform();
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	params.Owner = this;
	
	//错误 不应该使用 DefaultWeapon->GetClass()，传入的Class信息会变成 TSubclassOf的UClass
	TObjectPtr<AActor> defaultWeapon = GetWorld()->SpawnActor(DefaultWeapon,&ActorTransform,params);
	ABaseWeapon* weapon = Cast<ABaseWeapon>(defaultWeapon);
	if(defaultWeapon && weapon)
	{
		weapon->EquipItem();
	}
}

void ABaseEnemy::ShowHPBar()
{
	HPBarWidget->GetWidget()->SetVisibility(ESlateVisibility::Visible);
}

void ABaseEnemy::HideHPBar()
{
	HPBarWidget->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
}

