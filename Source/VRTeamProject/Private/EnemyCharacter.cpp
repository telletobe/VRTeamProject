// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//기본 값 세팅
	SetCurrentHp(GetMaxHp());
	SetDef(1.0f);
	SetAtk(3.0f);

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));

	// NavInvoker 관련 생성,삭제 변수값 설정
	NavGenerationRadius = 2000.0f;
	NavRemovalRadius = 500.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);


	UCapsuleComponent* EnemyCollision = GetCapsuleComponent();
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	USkeletalMeshComponent* EnemyMesh = GetMesh();
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void AEnemyCharacter::FindSpawnPoint()
{
	TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (auto StartPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(StartPoint))
		{
			if (!SpawnPoint)
			{
				if (StartPoint->ActorHasTag(TEXT("EnemySpawnPoint")))
				{
					SpawnPoint = StartPoint;
				}

			}
		}
	}
}


void AEnemyCharacter::FindDeSpawnPoint()
{
TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (auto EndPoint : FoundEndPoint)
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

	GetCapsuleComponent()->SetVisibility(false);
	GetMesh()->SetVisibility(false);
	PrimaryActorTick.bCanEverTick = false;
	bIsActive = false;
}

void AEnemyCharacter::Spawn()
{
	bIsActive = true;

	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Spawn() called!"));

	if (IsValid(SpawnPoint))
	{
		SetCurrentHp(GetMaxHp());
		SetActorLocation(SpawnPoint->GetActorLocation());
	}

	GetCapsuleComponent()->SetVisibility(true);
	GetMesh()->SetVisibility(true);
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
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
	CurrentHp = EnemyHp;
}

void AEnemyCharacter::SetMaxHp(float EnemyHp)
{
	MaxHp = EnemyHp;
}

void AEnemyCharacter::SetDef(float EnemyDef)
{
	Def = EnemyDef;
}

void AEnemyCharacter::SetAtk(float EnemyAtk)
{
	Atk = EnemyAtk;
}
