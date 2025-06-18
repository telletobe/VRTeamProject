// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>
#include <VRProjectGameModeBase.h>
#include "Engine/TargetPoint.h"

int32 AEnemySpawner::EnemyPoolSize = 20;

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	CurrentKillCnt = 1;
	RequiredKillCnt = 10;
	CreateDelay = 0.1f;
	SpawnDelay = 0.7f;
	PoolIndex = EnemyPoolSize - 1;
}


//void AEnemySpawner::CreateEnemySpawner()
//{
//	if (BPEnemySpawner)
//	{
//		AEnemySpawner* Spanwer = GetWorld()->SpawnActor<AEnemySpawner>(BPEnemySpawner,FVector(-1350.0f,3200.0f,350.0f),FRotator(0,0,0));
//	}
//}


void AEnemySpawner::CreateEnemy()
{
	FVector SpawnPoint = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());

	bool bIsSpawn = true;

	if (bIsSpawn)
	{
		TSubclassOf<AEnemyCharacter> CommonEnemy = LoadClass<AEnemyCharacter>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Character/Enemy/BP_EnemyCharacter.BP_EnemyCharacter_C'"));
		if (CommonEnemy)
		{

			if (EnemyPool.Num() < EnemyPoolSize)
			{
				AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(CommonEnemy, SpawnPoint, FRotator(0));

				if (SpawnedEnemy)
				{
					SpawnedEnemy->SpawnDefaultController();
					SpawnedEnemy->DeSpawn();
					SpawnedEnemy->OnEnemyDespawned.AddDynamic(this, &AEnemySpawner::CheckGameClear);
					SpawnedEnemy->OnEnemyDeath.AddDynamic(this,&AEnemySpawner::IncreaseKillCount);
					EnemyPool.Add(SpawnedEnemy);
				}				
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(CreateHandle);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("InValid Enemy!"));
		}
	}
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyPool.Num() == 0) return;

	for (int32 i = 0; i < EnemyPool.Num(); ++i)
	{
		int32 Index = (PoolIndex + i) % EnemyPool.Num();
		if (!EnemyPool[Index]->IsActive())
		{
			EnemyPool[Index]->Spawn();
			PoolIndex = (Index + 1) % EnemyPool.Num();
			return;
		}
	}
}

void AEnemySpawner::DeActivateEnemySpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);

	if (EnemyPool.Num() != 0)
	{
		for (AEnemyCharacter* SpawnedEnemy : EnemyPool)
		{
			if (SpawnedEnemy->IsActive())
			{
				SpawnedEnemy->DeSpawn();
			}
		}
	}
}

void AEnemySpawner::ActivateEnemySpawner()
{
	GameMode->TriggerGameStart();
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}


void AEnemySpawner::CheckGameClear()
{
	if (IsValid(GameMode))
	{

		if (!(GameMode->IsClear()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode:Player Alive : %s"), GameMode->IsPlayerAlive() ? TEXT("true") : TEXT("False"));
			if (CurrentKillCnt >= RequiredKillCnt)
			{			
				GameMode->TriggerGameClear();
				CurrentKillCnt = 1;

			}
			 if (!(GameMode->IsPlayerAlive()))
	  		 {
				 DeActivateEnemySpawner();
				 CurrentKillCnt = 1;
			 }
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode Is not Valid"));
		return;
	}
}

void AEnemySpawner::IncreaseKillCount()
{
	CurrentKillCnt++;
}



// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());

	CurrentKillCnt = 0;

	GetWorld()->GetTimerManager().SetTimer(CreateHandle,this,&AEnemySpawner::CreateEnemy,CreateDelay,true);
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SpawnEnemy();

}

