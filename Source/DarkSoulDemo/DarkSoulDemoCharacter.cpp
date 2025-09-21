// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkSoulDemoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Component/CharacterCombat.h"
#include "Component/CharacterState.h"
#include "Component/CharacterStats.h"
#include "Equipment/BaseWeapon.h"
#include "Interface/Interact.h"
#include "Kismet/GameplayStatics.h"
#include "DarkSoulTraceChannels.h"
#include "Component/Targeting.h"
#include "Interface/CanTargeting.h"
#include "DarkSoulSystemLibrary.h"
#include "EDamageType.h"
#include "Component/PotionInventory.h"
#include "DamageType/DarkSoulDamageType.h"
#include "Interface/SyncMsgToAnim.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADarkSoulDemoCharacter

void ADarkSoulDemoCharacter::ActivateCollision(EWeaponCollisionType WeaponCollisionType)
{
	CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
}

void ADarkSoulDemoCharacter::DeactiveCollision(EWeaponCollisionType WeaponCollisionType)
{
	CombatComponent->GetMainWeapon()->DeactiveCollision(WeaponCollisionType);
}

float ADarkSoulDemoCharacter::PerformAttack(EMontageAction AttackType)
{
	return 0;
}

void ADarkSoulDemoCharacter::Parried(AActor* Actor)
{
	//todo 
}

ADarkSoulDemoCharacter::ADarkSoulDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CombatComponent = CreateDefaultSubobject<UCharacterCombat>(TEXT("Combat"));
	StateComponent = CreateDefaultSubobject<UCharacterState>(TEXT("State"));
	StatsComponent = CreateDefaultSubobject<UCharacterStats>(TEXT("Stats"));
	TargetingComponent = CreateDefaultSubobject<UTargeting>(TEXT("Targeting"));
	PotionComponent = CreateDefaultSubobject<UPotionInventory>(TEXT("PotionComponent"));
	
}

void ADarkSoulDemoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	StateComponent->AddState(Player_State_GeneralAction);
	EquipDefaultWeapon();
	CurrentCombatType = ECombatType::Fists;
	StatsComponent->DeathEvent.AddUObject(this,&ThisClass::HandleDeathEvent);
	
	//在Blueprint中创建HUD
	OnTakePointDamage.AddDynamic(this,&ThisClass::TakePointDamage);
	OnTakeRadialDamage.AddDynamic(this,&ThisClass::TakeRadialDamage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this,&ThisClass::WatchMontagedEnd);
}

void ADarkSoulDemoCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
	OnTakePointDamage.RemoveDynamic(this,&ThisClass::TakePointDamage);
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this,&ThisClass::WatchMontagedEnd);
}

// UPlayerStatsModel* ADarkSoulDemoCharacter::GetPlayerStatsViewModel()
// {
// 	if(IsValid(PlayerStatsViewModel))
// 	{
// 		return PlayerStatsViewModel;
// 	}
// 	
// 	PlayerStatsViewModel = NewObject<UPlayerStatsModel>(this);
// 	PlayerStatsViewModel->InitializeWithStatsComponent(this->StatsComponent);
// 	return PlayerStatsViewModel;
// }

void ADarkSoulDemoCharacter::RefreshCharacterStats()
{
	StatsComponent->ResetStats();
	PotionComponent->RefreshPotionNum();
}

// float ADarkSoulDemoCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
// 	AController* EventInstigator, AActor* DamageCauser)
// {
// 	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
// 	
// }

//////////////////////////////////////////////////////////////////////////
// Input

void ADarkSoulDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			PlayerInputSubSystem = Subsystem;
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADarkSoulDemoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADarkSoulDemoCharacter::Look);
		
		//Rolling&Sprinting 长按时候会触发Trigger 短触会翻滚
		EnhancedInputComponent->BindAction(RollAction,ETriggerEvent::Triggered,this,&ThisClass::Sprint);
		// EnhancedInputComponent->BindAction(RollAction,ETriggerEvent::Ongoing,this,&ThisClass::OnGoing);
		EnhancedInputComponent->BindAction(RollAction,ETriggerEvent::Completed,this,&ThisClass::OnSprintCompleted);
		
		EnhancedInputComponent->BindAction(RollAction,ETriggerEvent::Canceled,this,&ThisClass::Rolling);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started,this,&ThisClass::Interact);

		//ToggleWeapon
		EnhancedInputComponent->BindAction(ToggleWeaponAction,ETriggerEvent::Started,this,&ThisClass::ToggleWeapon);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction,ETriggerEvent::Started,this,&ThisClass::PerformanceAttack);
		EnhancedInputComponent->BindAction(AttackAction,ETriggerEvent::Triggered,this,&ThisClass::PerformanceSpecialAttack);
		
		EnhancedInputComponent->BindAction(HeavyAttackAction,ETriggerEvent::Started,this,&ThisClass::PerformanceHeavyAttack);

		//Focus Target
		EnhancedInputComponent->BindAction(TargetAction,ETriggerEvent::Started,this,&ThisClass::ToggleTarget);

		EnhancedInputComponent->BindAction(TargetLeftAction,ETriggerEvent::Started,this,&ThisClass::SwitchTargetLeft);
		EnhancedInputComponent->BindAction(TargetRightAction,ETriggerEvent::Started,this,&ThisClass::SwitchTargetRight);
		//Blocking
		EnhancedInputComponent->BindAction(BlockAction,ETriggerEvent::Started,this,&ThisClass::ToggleBlocking);
		EnhancedInputComponent->BindAction(BlockAction,ETriggerEvent::Completed,this,&ThisClass::ReleaseBlocking);
		EnhancedInputComponent->BindAction(ParryAction,ETriggerEvent::Started,this,&ThisClass::ToggleParry);

		EnhancedInputComponent->BindAction(ConsumeAction,ETriggerEvent::Started,this,&ThisClass::TogglePotion);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADarkSoulDemoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && StateComponent->CanMovement())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADarkSoulDemoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !TargetingComponent->isLockOnTarget())
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADarkSoulDemoCharacter::Rolling(const FInputActionValue& Value)
{
	bool bSuccess = StatsComponent->DecreaseStamina(10);
	if(!bSuccess)
	{
		return;
	}
	//Rolling的时候只能获得 一维的输入
	//需要判断角色 往哪个方向翻滚 需要 输入按键信息
	
	if(TargetingComponent->isLockOnTarget())
	{
		if(PlayerInputSubSystem && MoveAction)
		{
			FInputActionValue CurrentValue = PlayerInputSubSystem->GetPlayerInput()->GetActionValue(MoveAction);
			FVector2d InputMove = CurrentValue.Get<FVector2d>();
			UE_LOG(LogTemp,Log,TEXT("FInputActionValue x = %f y = %f"),InputMove.X,InputMove.Y);
			FString MontageSectionName;
			if(InputMove.Y >= 0.01f)
			{
				MontageSectionName = "Default";	
			}
			else if(InputMove.Y <= -0.01f)
			{
				MontageSectionName = "Back";
			}
			else if(InputMove.X >= 0.01f)
			{
				MontageSectionName = "Right";
			}
			else
			{
				MontageSectionName = "Left";
			}
			PlayAnimMontage(TargetedRollingAnimMontage,1.0f,FName(MontageSectionName));
		}
	}
	else
	{
		this->PlayAnimMontage(RollingAnimMontage,1.0f);
		StateComponent->AddState(Player_State_Rolling);
	}
}

void ADarkSoulDemoCharacter::Sprint(const FInputActionValue& Value)
{
	UE_LOG(LogTemp,Log,TEXT("Sprint Trigger"))
	bool bEnoughStamina = StatsComponent->CheckHasEnoughStamina(5.0);
	if(bEnoughStamina)
	{
		GetCharacterMovement()->MaxWalkSpeed = 750.0f;
		StatsComponent->PauseRegenerateStamina();
		StatsComponent->DecreaseStamina(0.1);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		StatsComponent->ResumeRegenerateStamina();
	}
	
}

void ADarkSoulDemoCharacter::OnSprintCompleted(const FInputActionValue& Value)
{
	UE_LOG(LogTemp,Log,TEXT("Sprint Trigger Completed"))
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	StatsComponent->ResumeRegenerateStamina();
}

void ADarkSoulDemoCharacter::Interact(const FInputActionValue& Value)
{
	const FVector actorLocation = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	//需要一次转换将ECC_GameTraceChannel1转换成EObjectTypeQuery
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Interaction));
	
	TArray<AActor*> IgnoreActor;

	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		actorLocation,
		actorLocation,
		100,
		ObjectTypes,
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if(!OutHit.bBlockingHit)
	{
		return;
	}
	
	AActor* hitActor = OutHit.GetActor();
	bool isInteract = hitActor->GetClass()->ImplementsInterface(UInteract::StaticClass());
	if(!isInteract)
	{
		return;
	}
	IInteract* interactObj = Cast<IInteract>(hitActor);
	if(interactObj == nullptr)
	{
		return;
	}
	interactObj->Interact(this);
}

void ADarkSoulDemoCharacter::ToggleWeapon(const FInputActionValue& Value)
{
	ToggleWeaponInner();
}

void ADarkSoulDemoCharacter::ToggleBlocking(const FInputActionValue& Value)
{
	bool hasMainShield = CombatComponent->GetMainShield() != nullptr;
	if(!hasMainShield)return;

	bool bIsEnough = StatsComponent->CheckHasEnoughStamina(5.0);
	if(!bIsEnough)return;

	if(!StateComponent->CanBlocking())return;
	
	CombatComponent->EnableBlock();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Implements<USyncMsgToAnim>())
	{
		ISyncMsgToAnim::Execute_SyncBlocking(AnimInstance,true);
	}
	StateComponent->AddState(Player_State_Blocking);
}

void ADarkSoulDemoCharacter::ReleaseBlocking(const FInputActionValue& Value)
{
	CombatComponent->DisableBlock();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Implements<USyncMsgToAnim>())
	{
		ISyncMsgToAnim::Execute_SyncBlocking(AnimInstance,false);
	}
	StateComponent->RemoveState(Player_State_Blocking);
}

void ADarkSoulDemoCharacter::ToggleParry(const FInputActionValue& Value)
{
	if(CombatComponent->HasMainShield() == false)return;

	if(StateComponent->CanParry() == false)return;

	const float ParryCost = CombatComponent->GetMainWeapon()->GetCostStamina(EMontageAction::Parry);
	bool hasEnoughStamina = StatsComponent->CheckHasEnoughStamina(ParryCost);
	if(!hasEnoughStamina)return;

	//扣除体力 暂停回复
	StatsComponent->DecreaseStamina(ParryCost);
	StatsComponent->PauseRegenerateStamina();
	//播放AnimMontage
	auto ParryTuple = CombatComponent->GetMainWeapon()->GetMontageForAction(EMontageAction::Parry);
	PlayAnimMontage(ParryTuple.Key,1.0);
	
}

bool ADarkSoulDemoCharacter::CanPerformanceAttack()
{
	bool bHasTag = StateComponent->GetGameplayContainer().HasTagExact(Player_State_Attacking);
	bool bHasWeapon = CombatComponent->GetMainWeapon() != nullptr;
	bool bEnoughStamina = StatsComponent->CheckHasEnoughStamina(15.0f);
	bool bEnableCombat = CombatComponent->CanEnableCombat();
	bool bIsBlocking = StateComponent->GetGameplayContainer().HasTagExact(Player_State_Blocking);
	UE_LOG(LogTemp,Log,TEXT("has Player_State_Attacking tag %d hasWeapon %d enoughStamina %d enableCombat %d"),
		!bHasTag,bHasWeapon,bEnoughStamina,bEnableCombat);
	return !bHasTag && bHasWeapon && bEnoughStamina && bEnableCombat && !bIsBlocking;	
}

bool ADarkSoulDemoCharacter::CanAutomaticallyDrawWeapon()
{
	bool bHasTag = StateComponent->GetGameplayContainer().HasTagExact(Player_State_Attacking);
	bool bHasWeapon = CombatComponent->GetMainWeapon() != nullptr;
	bool bEnableCombat = CombatComponent->CanEnableCombat();
	bool bIsBlocking = StateComponent->GetGameplayContainer().HasTagExact(Player_State_Blocking);
	UE_LOG(LogTemp,Log,TEXT("has Player_State_Attacking tag %d hasWeapon %d enableCombat %d"),
		!bHasTag,bHasWeapon,bEnableCombat);
	return !bHasTag && bHasWeapon && !bEnableCombat && !bIsBlocking;
}


void ADarkSoulDemoCharacter::StartDelayTime(float latent, FTimerDelegate::TMethodPtr<ADarkSoulDemoCharacter> delayCallback)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(TimerHandle.IsValid())
	{
		TimerManager.ClearTimer(TimerHandle);
	}
	TimerManager.SetTimer(TimerHandle,this,delayCallback,0.1,false,(latent-0.1f));
}

void ADarkSoulDemoCharacter::StartDelayTime(float latent, TFunction<void()>&& delayCallback)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(TimerHandle.IsValid())
	{
		TimerManager.ClearTimer(TimerHandle);
	}
	TimerManager.SetTimer(TimerHandle,MoveTemp(delayCallback),0.1f,false,(latent-0.1f));
}

void ADarkSoulDemoCharacter::PerformanceAttack(const FInputActionValue& Value)
{
	if(!CanPerformanceAttack())
	{
		UE_LOG(LogTemp,Log,TEXT("PerformanceAttack CanPerformanceAttack = false"));
		if(CanAutomaticallyDrawWeapon())
		{
			ToggleWeapon(Value);
		}
		return;
	}
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if(MovementComponent && MovementComponent->IsFalling())
	{
		HandleAttack(EMontageAction::AirAttack,Player_State_Attacking_LightAttack);
	}
	else
	{
		HandleAttack(EMontageAction::LightAttack,Player_State_Attacking_LightAttack);	
	}
}

void ADarkSoulDemoCharacter::PerformanceHeavyAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp,Log,TEXT("PerformanceHeavyAttack PerformanceHeavyAttack"));
	if(!CanPerformanceAttack())
	{
		UE_LOG(LogTemp,Log,TEXT("PerformanceHeavyAttack CanPerformanceAttack = false"));
		if(CanAutomaticallyDrawWeapon())
		{
			ToggleWeapon(Value);
		}
		return;
	}
	
	HandleAttack(EMontageAction::HeavyAttack,Player_State_Attacking_HeavyAttack);
}

void ADarkSoulDemoCharacter::PerformanceSpecialAttack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp,Log,TEXT("PerformanceHeavyAttack PerformanceHeavyAttack"));
	if(!CanPerformanceAttack())
	{
		UE_LOG(LogTemp,Log,TEXT("PerformanceHeavyAttack CanPerformanceAttack = false"));
		if(CanAutomaticallyDrawWeapon())
		{
			ToggleWeapon(Value);
		}
		return;
	}
	HandleAttack(EMontageAction::SpecialAttack,Player_State_Attacking_SpecialAttack);
}

void SwitchActorLockOnIcon(AActor* Actor,bool on)
{
	if(!IsValid(Actor))return;
	bool implemented = Actor->GetClass()->ImplementsInterface(UCanTargeting::StaticClass());
	if(implemented)
	{
		ICanTargeting* canTargeting = Cast<ICanTargeting>(Actor);
		canTargeting->OnTargeted(on);
	}
}

void ADarkSoulDemoCharacter::ToggleTarget(const FInputActionValue& Value)
{
	if(TargetingComponent->isLockOnTarget())
	{
		AActor* curTarget = TargetingComponent->GetCurrentLockOnTarget();
		SwitchActorLockOnIcon(curTarget,false);
		TargetingComponent->StopLockOnTarget();
		TargetCandidate.Empty();
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("Start Targeting"));
		TargetCandidate = TargetingComponent->FindLockOnTargets();
		AActor* closetTarget = TargetingComponent->FindClosetTarget(TargetCandidate);
		if(!IsValid(closetTarget))
		{
			return;
		}
		UE_LOG(LogTemp,Log,TEXT("Closet Actor = %s"),*closetTarget->GetName());
		TargetingComponent->StartLockOnTarget(closetTarget);
		SwitchActorLockOnIcon(closetTarget,true);
	}
}

void ADarkSoulDemoCharacter::SwitchTargetLeft(const FInputActionValue& Value)
{
	if(TargetCandidate.IsEmpty())return;
	if(!TargetingComponent->isLockOnTarget())return;
	//取消之前的锁定
	AActor* prevTargetActor = TargetingComponent->GetCurrentLockOnTarget();
	SwitchActorLockOnIcon(prevTargetActor,false);
	
	int TargetCandidateIndex = TargetCandidate.Find(prevTargetActor);
	TargetCandidateIndex -= 1;
	if(TargetCandidateIndex < 0)
	{
		TargetCandidateIndex = TargetCandidate.Num() - 1;
	}
	AActor* curTarget = TargetCandidate[TargetCandidateIndex];
	TargetingComponent->StartLockOnTarget(curTarget);
	SwitchActorLockOnIcon(curTarget,true);
	
}

void ADarkSoulDemoCharacter::SwitchTargetRight(const FInputActionValue& Value)
{
	if(TargetCandidate.IsEmpty())return;
	if(!TargetingComponent->isLockOnTarget())return;
	//取消之前的锁定
	AActor* prevTarget = TargetingComponent->GetCurrentLockOnTarget();
	SwitchActorLockOnIcon(prevTarget,false);
	
	int TargetCandidateIndex = TargetCandidate.Find(prevTarget);
	TargetCandidateIndex += 1;
	if(TargetCandidateIndex >= TargetCandidate.Num())
	{
		TargetCandidateIndex = 0;
	}
	AActor* curTarget = TargetCandidate[TargetCandidateIndex];
	TargetingComponent->StartLockOnTarget(curTarget);
	SwitchActorLockOnIcon(curTarget,true);
}

void ADarkSoulDemoCharacter::TogglePotion(const FInputActionValue& Value)
{
	if(!PotionComponent->HasPotion())return;
	
	bool bCanDrink = StateComponent->CanDrinkPotion();
	if(!bCanDrink)return;
	
	StateComponent->AddState(Player_State_Potion);
	PlayAnimMontage(DrinkPotion,1.0);
	//生成药瓶
	if(IsValid(PotionMesh))
	{
		PotionMesh->Destroy();
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PotionMesh = GetWorld()->SpawnActor(PotionBP,&this->GetActorTransform(),SpawnParameters);
	FAttachmentTransformRules Rules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true);
	PotionMesh->AttachToComponent(GetMesh(),Rules,FName("potionuse"));
}

void ADarkSoulDemoCharacter::HandleAttack(EMontageAction Action,FGameplayTag AttackTag)
{
	TObjectPtr<ABaseWeapon> MainWeapon = CombatComponent->GetMainWeapon();
	TTuple<TObjectPtr<UAnimMontage>,FName> AnimMontage = MainWeapon->GetMontageForAction(Action);
	if(!IsValid(AnimMontage.Key))return;
	
	StateComponent->DisableMovement();
	StateComponent->AddState(Player_State_Attacking);
	StatsComponent->PauseRegenerateStamina();

	
	const float costStamina = MainWeapon->GetCostStamina(Action);
	StatsComponent->DecreaseStamina(costStamina);
	

	const bool bSameAttack = StateComponent->GetGameplayContainer().HasTagExact(AttackTag);
	if(!bSameAttack)
	{
		MainWeapon->ResetCombo();
	}
	StateComponent->AddState(AttackTag);
	
	UE_LOG(LogTemp,Log,TEXT("MainWeapon Current Step %d"),MainWeapon->CurCombo());

	int index = AnimMontage.Key->GetSectionIndex(AnimMontage.Value);
	float duration = AnimMontage.Key->GetSectionLength(index);
	PlayAnimMontage(AnimMontage.Key,1.0,AnimMontage.Value);
	
	//https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-timers?application_version=4.27
	//设置一个latent function
	// StatsComponent->SetDelayResume(duration);
	StateComponent->SetAttackAction(Action);
	this->StartDelayTime(duration,[this,AttackTag]
	{
		StateComponent->SetAttackAction(EMontageAction::None);
		ClearStateAndRegenerateStamina(AttackTag);
	});
	
}

void ADarkSoulDemoCharacter::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	// const APawn* pawn = InstigatedBy->GetPawn();
	const UDarkSoulDamageType* dt = Cast<UDarkSoulDamageType>(DamageType);
	if(dt == nullptr)return;
	switch (dt->GetDamageType())
	{
	case EDamageType::HitBack:
		TakeHitBack(InstigatedBy,Damage);
		DoCameraShake();
		break;
	case EDamageType::KnockBack:
		TakeKnockBack(InstigatedBy,Damage);
		break;
	}
	// UGameplayStatics::PlaySoundAtLocation();
}

void ADarkSoulDemoCharacter::TakeRadialDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, FVector Origin,
	const FHitResult& HitInfo, AController* InstigatedBy,
	AActor* DamageCauser)
{
	const UDarkSoulDamageType* dt = Cast<UDarkSoulDamageType>(DamageType);
	if(dt == nullptr)return;
	switch (dt->GetDamageType())
	{
	case EDamageType::HitBack:
		TakeHitBack(InstigatedBy,Damage);
		break;
		
	case EDamageType::KnockBack:
		TakeKnockBack(InstigatedBy,Damage);
		break;
	}
}

void ADarkSoulDemoCharacter::HandleDeathEvent()
{
	// StateComponent->ClearAllState();
	StateComponent->AddState(Player_State_Death);
	//忽略Pawn的碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	//开启布娃娃系统模拟物理
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
}

void ADarkSoulDemoCharacter::WatchMontagedEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if(bInterrupted)
	{
		UE_LOG(LogTemp,Log,TEXT("%s play Montage name %s bInterrupted %d"),*GetName(),*Montage->GetName(),bInterrupted)

		const FGameplayTagContainer& container = StateComponent->GetGameplayContainer();
        bool bIsAttacking = container.HasTag(Player_State_Attacking);

		UE_LOG(LogTemp,Log,TEXT("%s state is attacking %d"),*GetName(),bIsAttacking)
		if(bIsAttacking)
		{
			StateComponent->EnableMovement();
		}
	}
	if(Montage->GetName() == "AM_HitReact")
	{
		StateComponent->RemoveState(Player_State_Hit);
		StateComponent->EnableMovement();
	}
	else if(Montage->GetName() == "AM_RollUE5")
	{
		StateComponent->RemoveState(Player_State_Rolling);
	}
	else if(Montage->GetName() == "AM_Parry")
	{
		StatsComponent->ResumeRegenerateStamina();
	}
	else if(Montage->GetName() == "AM_DrinkPotion")
	{
		StateComponent->RemoveState(Player_State_Potion);
		if(IsValid(PotionMesh)) PotionMesh->Destroy();
	}
	else if(Montage->GetName() == "AM_KnockDown_Getup")
	{
		StateComponent->RemoveState(Player_State_Hit);
		StateComponent->EnableMovement();
	}
}

bool ADarkSoulDemoCharacter::IsFacingActor(const AActor* DamageInstigator)
{
	//判断敌人位置是否在正面
	FVector selfForward = GetActorForwardVector().GetSafeNormal();
	FVector InstigatorLocation =  DamageInstigator->GetActorLocation();
	FVector InstigatorForward = (InstigatorLocation - GetActorLocation()).GetSafeNormal();
	float degress = FVector::DotProduct(selfForward,InstigatorForward);
	return degress >= 0.5f;
}

void ADarkSoulDemoCharacter::TakeHitBack(AController* InstigatedBy,float Damage)
{
	bool canBlock = CanBlockAttack(InstigatedBy->GetPawn());
	if(canBlock)
	{
		//如果当前在防御 那就播放防御动画
		auto MontageTuple = CombatComponent->GetMainWeapon()->GetMontageForAction(EMontageAction::BlockReaction);
		PlayAnimMontage(MontageTuple.Key,1.0f);
		float costStamina = CombatComponent->GetMainWeapon()->GetCostStamina(EMontageAction::BlockReaction);
		StatsComponent->DecreaseStamina(costStamina);	
	}

	bool isParring = StateComponent->HasStateExact(Player_State_Parrying);
	bool isFacing = IsFacingActor(InstigatedBy->GetPawn());
	if(isParring && isFacing)
	{
		UE_LOG(LogTemp,Log,TEXT("Parry %s Attack"),*InstigatedBy->GetPawn()->GetName())
		AActor* Actor = InstigatedBy->GetPawn();
		const bool isImplemented = Actor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass());
		if(isImplemented)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor);
			CombatInterface->Parried(this);
		}
		return;
	}
	
	//计算角度
	AActor* InstigatedActor = InstigatedBy->GetPawn();
	FVector InstigatedLocation = InstigatedActor->GetActorLocation();
	FVector Direction = InstigatedLocation - GetActorLocation();
	float degress = UDarkSoulSystemLibrary::CalculateZAxisRotation(Direction,GetActorForwardVector());
		
	EHitDirection edir = UDarkSoulSystemLibrary::ConvertToHitDirection(degress);
	auto MontageTuple = CombatComponent->GetMainWeapon()->GetMontageForAction(EMontageAction::HitReaction);
	//根据角度 获取对应受击动画
	FName HitDirectionName = HitDirectionDisplayName(edir);
	PlayAnimMontage(MontageTuple.Key,1.0f,HitDirectionName);
	StatsComponent->TakeDamage(Damage);
	StateComponent->AddState(Player_State_Hit);
	StateComponent->DisableMovement();
}

void ADarkSoulDemoCharacter::TakeKnockBack(AController* InstigatedBy,float Damage)
{
	StatsComponent->TakeDamage(Damage);
	StateComponent->AddState(Player_State_Hit);
	StateComponent->DisableMovement();
	PlayAnimMontage(KnockBack,1.0f);
}

void ADarkSoulDemoCharacter::DoCameraShake()
{
	UGameplayStatics::PlayWorldCameraShake(this,CameraShakeClass,GetActorLocation(),InnerRadius,OuterRadius,Falloff);
}

bool ADarkSoulDemoCharacter::CanBlockAttack(const AActor* DamageInstigator)
{
	//判断敌人位置是否在正面
	bool isFacing = IsFacingActor(DamageInstigator);
	//判断当前状态是否在防御
	bool isBlockingState = StateComponent->HasStateExact(Player_State_Blocking);
	//体力是否足够
	bool hasEnoughStamina = StatsComponent->CheckHasEnoughStamina(5.0);
	
	return isFacing && isBlockingState && hasEnoughStamina;
}


void ADarkSoulDemoCharacter::ClearStateAndRegenerateStamina(FGameplayTag removeTag)
{
	StatsComponent->ResumeRegenerateStamina();
	StateComponent->RemoveState(removeTag);
	StateComponent->RemoveState(Player_State_Attacking);
}

void ADarkSoulDemoCharacter::EquipDefaultWeapon()
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
		CombatComponent->SetWeapon(weapon);
		weapon->EquipItem();
	}
}

void ADarkSoulDemoCharacter::ToggleWeaponInner()
{
	if(!CombatComponent->GetMainWeapon())
	{
		return;
	}
	
	StateComponent->AddState(Player_State_GeneralAction);

	TTuple<TObjectPtr<UAnimMontage>,FName> anim;
	ECombatType nextCombatType = ECombatType::None;
	if(CombatComponent->CanEnableCombat())
	{
		anim = CombatComponent->GetMainWeapon()->GetMontageForAction(EMontageAction::Unequip);
		
	}
	else
	{
		anim = CombatComponent->GetMainWeapon()->GetMontageForAction(EMontageAction::Equip);
		nextCombatType = CombatComponent->GetMainWeapon()->GetCombatType();

		const bool isOneHandSword = CombatComponent->GetMainWeapon()->GetCombatType() == ECombatType::SingleSword;
		const bool hasShield = CombatComponent->GetMainShield() != nullptr;
		if(isOneHandSword && hasShield)
		{//单手剑的情况下,检查下是否有盾
			nextCombatType = ECombatType::SwordShield;
		}
	}
	//播放montage通过ABP中的slot
	//基础slot 使用 layered blend per bone混合基础动画
	//需要设置Layered blend per bone节点中的BranchFilter,指定混合骨骼起始位置
	float latent = PlayAnimMontage(anim.Key,1.0f,anim.Value);
	UE_LOG(LogTemp,Log,TEXT("Start ToggleWeapon %f"),latent);
	StartDelayTime(latent,[this,nextCombatType]()
	{
		UE_LOG(LogTemp,Log,TEXT("Finish ToggleWeapon nextCombatType %d"),nextCombatType);
		CurrentCombatType = nextCombatType;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(IsValid(AnimInstance))
		{
			bool bImplemented = AnimInstance->Implements<USyncMsgToAnim>();
			if(bImplemented)
			{
				ISyncMsgToAnim::Execute_SyncCombatMode(AnimInstance,CurrentCombatType);
			}
		}
	});
}
