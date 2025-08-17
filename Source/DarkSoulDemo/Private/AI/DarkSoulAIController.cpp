// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DarkSoulAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseEnemy.h"
#include "Component/CharacterRotator.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
ADarkSoulAIController::ADarkSoulAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

// Called when the game starts or when spawned
void ADarkSoulAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADarkSoulAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ABaseEnemy* enemy = Cast<ABaseEnemy>(InPawn);
	RunBehaviorTree(enemy->GetBehaviorTree());
}

void ADarkSoulAIController::UpdateTarget()
{
	TArray<AActor*> outActors;
	AIPerception->GetKnownPerceivedActors(nullptr,outActors);
	AController* firstController = GetWorld()->GetFirstPlayerController();
	ACharacter* character = firstController->GetCharacter();
	
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(GetPawn());
	if(!IsValid(BaseEnemy))
	{
		UE_LOG(LogTemp,Error,TEXT("ADarkSoulAIController::UpdateTarget Character is not ABaseEnemy"))
		return;
	}
	UCharacterRotator* CharacterRotator = BaseEnemy->GetComponentByClass<UCharacterRotator>();
	if(!IsValid(CharacterRotator))
	{
		return;
	}
	if(outActors.Contains(character))
	{
		GetBlackboardComponent()->SetValueAsObject(FName("Target"),character);
		BaseEnemy->ShowHPBar();
		CharacterRotator->UpdateTarget(character);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(FName("Target"),nullptr);
		BaseEnemy->HideHPBar();
		CharacterRotator->UpdateTarget(nullptr);
	}
}

// Called every frame
void ADarkSoulAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	invertal -= DeltaTime;
	if(invertal <= 0.f)
	{
		UpdateTarget();
		invertal = 0.1f;
	}
}

