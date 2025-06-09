// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>
#include <VRProjectGameModeBase.h>

int32 AEnemySpawner::EnemyPoolSize = 20;

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

}


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
					SpawnedEnemy->OnEnemyDied_Delegate.AddDynamic(this, &AEnemySpawner::CheckGameClear);
				}

				EnemyPool.Add(SpawnedEnemy);

			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(CreateHandle);
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Fin"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("InValid Enemy!"));
		}
	}
}

void AEnemySpawner::CheckGameClear(AEnemyCharacter* Enemy)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("Spawner : CheckGameClear Call"));

	if (CurrentKillCnt == RequiredKillCnt)
	{
		GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->TriggerGameClear();
	}
	else
	{
		CurrentKillCnt++;
	}
}



// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	CurrentKillCnt = 0;

	GetWorld()->GetTimerManager().SetTimer(CreateHandle,this,&AEnemySpawner::CreateEnemy,CreateDelay,true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SpawnEnemy();

}

