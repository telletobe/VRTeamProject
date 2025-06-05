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
	SetSpawnDelay(2.0f);

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
	TArray<AActor*> RandomPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (auto StartPoint : FoundEndPoint)
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
	if (GetCurrentHp() <= 0)
	{
		Die();
	}

	if (IsValid(DeSpawnPoint))
	{
		SetActorLocation(DeSpawnPoint->GetActorLocation());
	}

	SetActorHiddenInGame(true);
	PrimaryActorTick.bCanEverTick = false;
	bIsActive = false;

	GetWorld()->GetTimerManager().SetTimer(SpawnHandle,this,&AEnemyCharacter::Spawn,GetSpawnDelay(), false);
}

void AEnemyCharacter::Spawn()
{
	SetActorHiddenInGame(false);
	bIsActive = true;

	GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);

	if (IsValid(SpawnPoint))
	{
		SetCurrentHp(GetMaxHp());
		SetActorLocation(SpawnPoint->GetActorLocation());
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::Die()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::MakeRandomColor(),TEXT("Enemy Die!"));
	OnEnemyDied_Delegate.Broadcast(this);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FindSpawnPoint();
	FindDeSpawnPoint();
	Spawn();
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
