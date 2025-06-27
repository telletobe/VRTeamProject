// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>
#include <VRProjectGameModeBase.h>
#include "Engine/TargetPoint.h"

/*
	메모리 해제는 게임모드에서 처리
*/

const int32 AEnemySpawner::EnemyPoolSize = 20;
const int32 AEnemySpawner::RequiredKillCnt = 20;
int32 AEnemySpawner::CurrentKillCnt = 1;

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	CreateDelay = 0.1f;
	SpawnDelay = 0.7f;
	PoolIndex = EnemyPoolSize - 1;
}

void AEnemySpawner::CreateEnemy()
{
	//메모리에 Enemy를 할당하는 코드
	const FVector SpawnPoint = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
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
					SpawnedEnemy->OnEnemyKilled.AddDynamic(this,&AEnemySpawner::IncreaseKillCount);
					SpawnedEnemy->OnEnemyDeSpawn.AddDynamic(this, &AEnemySpawner::CheckGameClear);
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
	// 메모리에 할당되어있는 Enemy를 활성화 시키는 코드
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
	//메모리에 할당은 유지하면서, Enemy를 보이지않게 함.
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
	//EnemySpawner의 동작 활성코드
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}

void AEnemySpawner::CheckGameClear()
{
	//적이 죽을 때, 게임모드의 클리어 상태를 체크해서 킬카운트를 증가. 
	//조건에 따라 게임을 클리어, 혹은 플레이어 사망으로 인한 스포너 비활성 처리
	UE_LOG(LogTemp, Warning, TEXT("CurrentKillCnt : %d , RequiredKillCnt : %d"), CurrentKillCnt, RequiredKillCnt);
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
	if (GameMode)
	{
		GameMode->OnRestart.AddUniqueDynamic(this,&AEnemySpawner::ActivateEnemySpawner);
	}

	CurrentKillCnt = 0;

	if (CreateDelay <= 0)
	{
		CreateDelay = 0.1f;
	}

	if (SpawnDelay <= 0)
	{
		SpawnDelay = 0.7f;
	}

	GetWorld()->GetTimerManager().SetTimer(CreateHandle,this,&AEnemySpawner::CreateEnemy,CreateDelay,true); // Enemy 메모리 할당
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true); // Enemy 활성화
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

