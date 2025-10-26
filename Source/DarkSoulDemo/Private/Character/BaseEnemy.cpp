// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseEnemy.h"

#include "BrainComponent.h"
#include "EnemyManagerSubsystem.h"
#include "Component/CharacterState.h"
#include "Component/CharacterStats.h"
#include "Component/CharacterCombat.h"
#include "Component/CharacterRotator.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Controller/EnemyAIController.h"
#include "Equipment/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"


// Sets default values
ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StateComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterState>(this,TEXT("State"));
	StatsComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterStats>(this,TEXT("Stats"));
	CombatComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterCombat>(this,TEXT("Combat"));
	RotatorComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterRotator>(this,TEXT("Rotator"));
	
	LockOnWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this,TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(RootComponent);

	HPBarWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this,TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(RootComponent);

	ShowWeaponMeshEditor = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,TEXT("ShowWeaponMeshEditor"));
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
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this,&ThisClass::ListenAnimMontageFinish);

	UEnemyManagerSubsystem* Subsystem = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if(Subsystem)
	{
		Subsystem->RegisterEnemy(this);
	}
}

void ABaseEnemy::BeginDestroy()
{
	Super::BeginDestroy();
	if(GetMesh() && GetMesh()->GetAnimInstance())
	{
		if(GetMesh()->GetAnimInstance()->OnMontageEnded.IsBound())
		{
			GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this,&ThisClass::ListenAnimMontageFinish);	
		}
	}

	UE_LOG(LogTemp,Log,TEXT("ABaseEnemy::BeginDestroy"))
	// WorldSubsystem会先于Actor的BeginDestroy销毁
	// if(GetWorld())
	// {
	// 	UEnemyManagerSubsystem* Subsystem = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	// 	if(Subsystem)
	// 	{
	// 		Subsystem->UnregisterEnemy(this);
	// 	}	
	// }
	
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
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// SetActorEnableCollision(false);
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	//开启布娃娃系统模拟物理
	if(bNeedRagDoll)
	{
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}
	
	GetCharacterMovement()->DisableMovement();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetHiddenInGame(true);
	
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());
	if(IsValid(controller))
	{
		controller->GetBrainComponent()->StopLogic(FString("Enemy is Dead"));
	}
	HideHPBar();
	
	CombatComponent->DisableCollision();
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

void ABaseEnemy::ListenAnimMontageFinish(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp,Log,TEXT("ABaseEnemy::ListenAnimMontageFinish %s bInterrupted %d "),*Montage->GetName(),bInterrupted)
	if(Montage->GetName() == "AM_Parried")
	{
		StateComponent->RemoveState(Player_State_Parried);
	}
	if(Montage->GetName() == "AM_Boss_TwoHandSpecial")
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	if(Montage->GetName() == "AM_StealthKillVictim" || Montage->GetName() == "AM_RiposteVictim" )
	{
		StopAnimMontage();
		StatsComponent->TakeDamage(999999999.0f);
	}
	StatsComponent->ResumeRegenerateStamina();
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
	float costStamina = MainWeapon->GetCostStamina(AttackType);
	StatsComponent->DecreaseStamina(costStamina);
	StatsComponent->PauseRegenerateStamina();
	if(AttackType == EMontageAction::SpecialAttack)
	{
		int numSection = AnimMontage.Key->GetNumSections();
		int index = FMath::RandRange(0,numSection);
		PlayAnimMontage(AnimMontage.Key,1.0,AnimMontage.Key->GetSectionName(index));
		float duration = AnimMontage.Key->GetSectionLength(index);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		return duration;
	}
	
	
	if(!IsValid(AnimMontage.Key)) return 0;
	int sectionIndex = AnimMontage.Key->GetSectionIndex(AnimMontage.Value);
	float duration = AnimMontage.Key->GetSectionLength(sectionIndex);
	PlayAnimMontage(AnimMontage.Key,1.0,AnimMontage.Value);
	return duration;
}

void ABaseEnemy::Parried(AActor* Actor)
{
	StateComponent->AddState(Player_State_Parried);
	StopAnimMontage();
	PlayAnimMontage(AM_Parried,1.0);
}

bool ABaseEnemy::CanBeStealthKilled()
{
	return CanBeStealthKilledForBP();
}

void ABaseEnemy::StealthKilled()
{
	AController* controller = GetController();
	AAIController* AIController = Cast<AAIController>(controller);
	AIController->GetBrainComponent()->StopLogic(TEXT("Stealth Killed"));
	StopAnimMontage(nullptr);
	GetCharacterMovement()->DisableMovement();
	PlayAnimMontage(BeStealthKill);
	// StatsComponent->TakeDamage(999999999);
	bNeedRagDoll = false;
}

void ABaseEnemy::RiposteKilled()
{
	AController* controller = GetController();
	AAIController* AIController = Cast<AAIController>(controller);
	AIController->GetBrainComponent()->StopLogic(TEXT("Riposte Killed"));
	StopAnimMontage(nullptr);
	GetCharacterMovement()->DisableMovement();
	PlayAnimMontage(RiposteVictim);
	// StatsComponent->TakeDamage(999999999);
	bNeedRagDoll = false;
}

bool ABaseEnemy::CanBeRiposteKilled()
{
	if(!CanBeRiposteKilledForBP())return false;
	if(StatsComponent->IsDead())return false;
	return StateComponent->HasStateExact(Player_State_Stunned);

}

void ABaseEnemy::EquipDefaultWeapon()
{
	if(!DefaultWeapon)return;

	ABaseWeapon* weapon = CombatComponent->GetMainWeapon();
	if(!IsValid(weapon))
	{
		const FTransform& ActorTransform = GetActorTransform();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
		params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		params.Owner = this;
	
		//错误 不应该使用 DefaultWeapon->GetClass()，传入的Class信息会变成 TSubclassOf的UClass
		TObjectPtr<AActor> defaultWeapon = GetWorld()->SpawnActor(DefaultWeapon,&ActorTransform,params);
		weapon = Cast<ABaseWeapon>(defaultWeapon);
		if(defaultWeapon && weapon)
		{
			weapon->EquipItem();
			CombatComponent->SetWeapon(weapon);
		}	
	}
	else
	{
		weapon->EquipItem();
	}
	
}

void ABaseEnemy::UnequipDefaultWeapon()
{
	if(!DefaultWeapon)return;

	ABaseWeapon* weapon = CombatComponent->GetMainWeapon();
	if(!IsValid(weapon))return;
	weapon->UnequipItem();
}

// void ABaseEnemy::ShowHPBar()
// {
// 	
// }
//
// void ABaseEnemy::HideHPBar()
// {
// 	
// }

void ABaseEnemy::ShowHPBar_Implement()
{
	HPBarWidget->GetWidget()->SetVisibility(ESlateVisibility::Visible);
}

void ABaseEnemy::HideHPBar_Implement()
{
	HPBarWidget->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
}

