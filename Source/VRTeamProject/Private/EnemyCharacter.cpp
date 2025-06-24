// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyAIController.h>
#include "PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//기본 값 세팅
	SetCurrentHp(1);
	SetDef(1.0f);
	SetAtk(3.0f);
	SetSpawnDelay(2.0f);

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));

}


void AEnemyCharacter::FindSpawnPoint()
{
	TArray<AActor*> FoundEndPoint;
	TArray<AActor*> RandomPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (const auto& StartPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(StartPoint))
		{
			if (!SpawnPoint)
			{
				if (StartPoint->ActorHasTag(TEXT("EnemySpawnPoint")))
				{
					RandomPoint.Add(StartPoint);
					
				}

			}
		}
	}

	SpawnPoint = RandomPoint[FMath::RandRange(0,RandomPoint.Num()-1)];
}


void AEnemyCharacter::FindDeSpawnPoint()
{
	TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (const auto& EndPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(EndPoint))
		{
			if (!DeSpawnPoint)
			{
				if (EndPoint->ActorHasTag(TEXT("EnemyDeSpawnPoint")))
				{
					DeSpawnPoint = EndPoint;
				}

			}
		}
	}
}

void AEnemyCharacter::DeSpawn()
{

	if (IsValid(DeSpawnPoint))
	{
		SetActorLocation(DeSpawnPoint->GetActorLocation());
	}
	
	SetActorHiddenInGame(true);
	bIsActive = false;
	NotifyEnemyDespawn();
	SetActorTickEnabled(false);
}


void AEnemyCharacter::Spawn()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	this->SetActorEnableCollision(true);
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
	APlayerCharacter* Player = Cast<APlayerCharacter>(EnemyController->GetTargetActor());
	if (Player)
	{
		EnemyController->StartChasing(Player);
	}

	bIsActive = true;
	bIsDeathAnim = false;

	if (IsValid(SpawnPoint))
	{
		SetCurrentHp(GetMaxHp());
		SetActorLocation(SpawnPoint->GetActorLocation());
	}
}

void AEnemyCharacter::NotifyEnemyDespawn()
{
	OnEnemyDespawned.Broadcast();
}

void AEnemyCharacter::NotifyEnemyDeath()
{
	OnEnemyDeath.Broadcast();
}


void AEnemyCharacter::PlayDeathMontage()
{

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Anim"));

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (DeathMontage)
			{
				AnimInstance->Montage_Play(DeathMontage);
				bIsDeathAnim = true;
				AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
				EnemyController->StopMovement();
				this->SetActorEnableCollision(false);

				// 몽타주 끝나고 처리될 함수 등록 (타이머 방식)
				float Duration = DeathMontage->GetPlayLength()-0.5f;
				GetWorld()->GetTimerManager().SetTimer(DeathAnimTimerHandle,this,&AEnemyCharacter::DeathMontageEnded,Duration,false);
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Anim"));
			}
		}
	}
}

void AEnemyCharacter::DeathMontageEnded()
{
	  // 죽음 이후 숨기기 + 비활성 처리
    NotifyEnemyDeath(); // 필요 시
    DeSpawn();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// NavInvoker 관련 생성,삭제 변수값 설정
	NavGenerationRadius = 2000.0f;
	NavRemovalRadius = 500.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);


	UCapsuleComponent* EnemyCollision = GetCapsuleComponent();
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	USkeletalMeshComponent* EnemyMesh = GetMesh();
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FindSpawnPoint();
	FindDeSpawnPoint();
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemyCharacter::SetCurrentHp(float EnemyHp)
{
	if (EnemyHp < 0)
	{
		CurrentHp = GetMaxHp();
	}
	else
	{
		CurrentHp = EnemyHp;
	}
}

void AEnemyCharacter::SetMaxHp(float EnemyHp)
{
	if (EnemyHp < 0)
	{
		return;
	}
	else
	{
		MaxHp = EnemyHp;
	}
}

void AEnemyCharacter::SetDef(float EnemyDef)
{
	if (EnemyDef < 0)
	{
		Def = DefaultDef;
	}
	else
	{
		Def = EnemyDef;
	}
}

void AEnemyCharacter::SetAtk(float EnemyAtk)
{
	if (EnemyAtk < 0)
	{

	}
	else
	{
		Atk = EnemyAtk;
	}
}

void AEnemyCharacter::SetSpawnDelay(float EnemySpawnDelay)
{
	SpawnDelay = EnemySpawnDelay;
}
