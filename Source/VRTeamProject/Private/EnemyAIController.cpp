// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include <EnemyCharacter.h>
#include "Perception/AISenseConfig_sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = DetectionRadius; //�����Ÿ�
	SightConfig->LoseSightRadius = LoseInterestRadius; // ��ǥ�� �Ҵ°Ÿ� (�����Ÿ����� Ŀ����)
	SightConfig->PeripheralVisionAngleDegrees = 360.0f; // �þ߰�
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (SightConfig && GetPerceptionComponent())
	{
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdate);
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{	
		TargetActor = PlayerCharacter;
	}
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	UpdateAIState();

	if (bIsChasing && TargetActor)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());

			if (Distance <= ChaseRadius)
			{
				/*�÷��̾��� �������� ���������� ��*/
				FVector PlayerLocation = TargetActor->GetActorLocation();
				FVector Dir = (PlayerLocation - ControlledPawn->GetActorLocation().GetSafeNormal());
				FVector Destination = PlayerLocation - Dir * 50.0f;  // �÷��̾� �������� 50cm ��

				MoveToLocation(TargetActor->GetActorLocation(), AcceptRadius);
			}
			else if (Distance > LoseInterestRadius)
			{
				StopMovement();
			}
		}
	}
}

void AEnemyAIController::StartChasing(AActor* Target)
{
	TargetActor = Target;
	bIsChasing = true;
}

void AEnemyAIController::StopChasing()
{
	bIsChasing = false;
	StopMovement();
}

void AEnemyAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Actor == PlayerCharacter)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			TargetActor = Actor;

			APawn* ControlledPawn = GetPawn();
			if (ControlledPawn)
			{
				float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), Actor->GetActorLocation());

				if (Distance <= ChaseRadius)
				{
					StartChasing(Actor);
				}
				else if (Distance <= DetectionRadius)
				{
					CurrentState = EAIState::Suspicious;
				}
			}
		}
	}
}

void AEnemyAIController::UpdateAIState()
{
	if (!TargetActor) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	float DistanceToTarget = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());

	switch (CurrentState)
	{
	case EAIState::Idle:
		if (DistanceToTarget <= DetectionRadius)
		{
			CurrentState = EAIState::Suspicious;
		}
		break;
	case EAIState::Suspicious:
		if (DistanceToTarget <= ChaseRadius)
		{
			StartChasing(TargetActor);
		}
		else if (DistanceToTarget > DetectionRadius)
		{
			CurrentState = EAIState::Idle;
		}
		break;
	case EAIState::Chasing:
		if (DistanceToTarget < LoseInterestRadius)
		{
			StopChasing();
		}
		break;
	}
}

